/*
 * MasterGameFile.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */

#include "MasterGameFile.h"

#include "MGFDataReader.h"
#include "StoneshipException.h"
#include "MasterGameFileManager.h"
#include "StoneshipConstants.h"

namespace Stoneship
{

	MasterGameFile::MasterGameFile(const String &filename, UID::Ordinal ordinal, MasterGameFileManager &manager)
	: mFilename(filename),
	  mOrdinal(ordinal),
	  mManager(manager),
	  mLoaded(false),
	  mFlags(0),
	  mDependencyCount(0),
	  mDependencies(nullptr),
	  mResourceCount(0),
	  mHints(nullptr),
	  mRecordCount(0),
	  mRecordGroupCount(0),
	  mCachedHint(nullptr)
	{
	}

	MasterGameFile::~MasterGameFile()
	{
		mInputStream.close();

		delete[] mDependencies;
		delete[] mHints;
	}

	void MasterGameFile::load()
	{
		mInputStream.open(mFilename.c_str(), std::ios::in | std::ios::binary);

		if(!mInputStream.good())
		{
			throw StoneshipException("File not found");
		}


		MGFDataReader ds(&mInputStream, this);

		uint64_t magic;
		ds.readULong(magic);
		if(magic != 0x46474D334750524E) // "NRPG3MGF"
		{
			throw StoneshipException("Invalid Magic ID");
		}

		String gameID;
		ds.readBString(gameID);
		if(gameID != STONESHIP_GAMEID)
		{
			throw StoneshipException("Incompatible game (Found: '" + gameID + "')");
		}


		ds.readUInt(mFlags);
		ds.readSString(mAuthor);
		ds.readSString(mDescription);


		//load and check for dependencies
		ds.readUShort(mDependencyCount);
		if(mDependencyCount > 0)
		{
			mDependencies = new Dependency[mDependencyCount];

			for(uint16_t i = 0; i < mDependencyCount; i++)
			{
				String filename;
				ds.readSString(filename);
				if(mManager.getLoadedMGF(filename) == nullptr)
				{
					throw StoneshipException("Dependency '" + filename + "' was not loaded before depending MGF");
				}

				mDependencies[i].filename = filename;
				ds.readUShort(mDependencies[i].ordinal);
			}
		}


		//define resource paths
		ds.readUShort(mResourceCount);
		for(uint16_t i = 0; i < mResourceCount; i++)
		{
			uint8_t resType;
			String resPath;

			ds.readUByte(resType);
			ds.readSString(resPath);

			//TODO: register resource paths
		}


		//load records
		ds.readUInt(mRecordGroupCount);

		mHeaderEndOfffset = ds.tell(); //store this so we may navigate quickly to start of records

		//skip through the file and index every top record group
		mHints = new OffsetHint[mRecordGroupCount];
		for(uint32_t i = 0; i < mRecordGroupCount; i++)
		{
			std::streampos offset = ds.tell();

			RecordHeader groupHeader;
			ds.readStruct(groupHeader);

			if(groupHeader.type != Record::TYPE_GROUP)
			{
				throw StoneshipException("Corrupted MGF found during scanning (expected Group record, found other)");
			}

			mHints[i].offset = offset;
			mHints[i].type = groupHeader.groupType;

			ds.skip(groupHeader.dataSize);
		}

		mLoaded = true;
	}

	const String &MasterGameFile::getFilename() const
	{
		return mFilename;
	}

	UID::Ordinal MasterGameFile::getOrdinal() const
	{
		return mOrdinal;
	}

	const String& MasterGameFile::getAuthor() const
	{
		return mAuthor;
	}

	uint16_t MasterGameFile::getDependencyCount() const
	{
		return mDependencyCount;
	}

	const MasterGameFile::Dependency *MasterGameFile::getDependencies() const
	{
		return mDependencies;
	}

	const String& MasterGameFile::getDescription() const
	{
		return mDescription;
	}

	uint32_t MasterGameFile::getRecordGroupCount() const
	{
		return mRecordGroupCount;
	}

	uint16_t MasterGameFile::getResourceCount() const
	{
		return mResourceCount;
	}

	bool MasterGameFile::isLoaded() const
	{
		return mLoaded;
	}

	UID::Ordinal MasterGameFile::localToGlobalOrdinal(UID::Ordinal local)
	{
		if(local == UID::SELF_REF_ORDINAL)
		{
			return getOrdinal();
		}

		for(uint16_t i = 0; i < mDependencyCount; i++)
		{
			if(mDependencies[i].ordinal == local)
			{
				MasterGameFile *refMgf = mManager.getLoadedMGF(mDependencies[i].filename);
				if(refMgf == nullptr)
				{
					throw StoneshipException("Supposedly loaded dependency was not loaded. This probably indicates the whole program is shit.");
				}

				return refMgf->getOrdinal();
			}
		}

		throw StoneshipException("Referenced ordinal not found in dependency table. This probably means a dependency was not loaded.");
	}

	RecordAccessor MasterGameFile::getRecord(UID::ID id)
	{
		MGFDataReader ds(&mInputStream, this);

		ds.seek(mHeaderEndOfffset);

		for(uint32_t i = 0; i < mRecordGroupCount; ++i)
		{
			RecordHeader groupHeader;
			ds.readStruct(groupHeader);
			if(groupHeader.type != Record::TYPE_GROUP)
			{
				throw StoneshipException("Corrupted MGF (expected top group record, found other)");
			}

			if(false) //does group type match the type mask? TODO: implement some matching here later
			{
				//no, skip it
				ds.skip(groupHeader.dataSize);
				continue;
			}

			ds.beginUnit(groupHeader.dataSize);
			while(ds.bytesRemainingInUnit())
			{
				RecordHeader recordHeader;
				ds.readStruct(recordHeader);

				if(recordHeader.id == id) // found it!
				{
					//leave stream pointer at data field; that's none of our business
					return RecordAccessor(recordHeader, &mInputStream, this);

				}else //this is not the record you are looking for
				{
					//skip until next record
					ds.skip(recordHeader.dataSize);
				}
			}
			ds.endUnit();
		}

		throw StoneshipException("Record not found in MGF");
	}

	RecordAccessor MasterGameFile::getRecord(UID::ID id, Record::Type type)
	{
		if(type == Record::TYPE_LOOKUP_ALL) //this fixes a problem with uninitialized Accessors. It's ugly, but it works for now
		{
			//typeless lookup

			return getRecord(id);
		}

		if(mCachedHint == nullptr || mCachedHint->type != type) //only search hint list when hint type differs from last search
		{
			//retrieve offset hint for record type
			for(uint32_t i = 0; i < mRecordGroupCount; ++i)
			{

				if(mHints[i].type == type)
				{
					mCachedHint = &(mHints[i]);

					goto hint_found; //whoooa velociraptor attack
				}
			}

			//no offset hint for that type -> this file does not contain that type of record
			throw StoneshipException("Record not found in MGF (missing hint)");
		}

	hint_found:

		MGFDataReader ds(&mInputStream, this);

		ds.seek(mCachedHint->offset);

		RecordHeader groupHeader;
		ds.readStruct(groupHeader);

		if(groupHeader.type != Record::TYPE_GROUP)
		{
			throw StoneshipException("Corrupted MGF (expected top group record, found other)");
		}

		ds.beginUnit(groupHeader.dataSize);
		while(ds.bytesRemainingInUnit())
		{
			RecordHeader recordHeader;
			ds.readStruct(recordHeader);

			if(recordHeader.id == id && recordHeader.type == type) // found it!
			{
				//leave stream pointer at data field; that's none of our business
				return RecordAccessor(recordHeader, &mInputStream, this);

			}else //this is not the record you are looking for
			{
				//skip until next record
				ds.skip(recordHeader.dataSize);
			}
		}

		throw StoneshipException("Record not found in MGF");
	}

}
