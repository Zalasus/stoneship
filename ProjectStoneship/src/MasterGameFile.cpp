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
#include "Entity.h"

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
	  mRecordGroupCount(0)
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
			STONESHIP_EXCEPT(StoneshipException::IO_ERROR, "File not found");
		}


		MGFDataReader ds(&mInputStream, this);

		uint64_t magic;
		ds.readULong(magic);
		if(magic != 0x46474D334750524E) // "NRPG3MGF"
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Invalid Magic ID");
		}

		String gameID;
		ds.readBString(gameID);
		if(gameID != STONESHIP_GAMEID)
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Incompatible game (Found: '" + gameID + "')");
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
					STONESHIP_EXCEPT(StoneshipException::DEPENDENCY_NOT_MET, "Dependency '" + filename + "' was not loaded before depending MGF");
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
				STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF found during scanning (expected Group record, found other)");
			}

			mHints[i].offset = offset;
			mHints[i].type = groupHeader.groupType;
			mHints[i].recordCount = groupHeader.recordCount;

			//there are some records which we want to index one by one like Modify. check if we have on of those on our hands
			if(mHints[i].type == Record::TYPE_MODIFY)
			{

				_indexModifies(mHints[i].recordCount);

			}else
			{
				//no special group here. just skip to the next one
				ds.skip(groupHeader.dataSize);
			}
		}

		mLoaded = true;
	}

	void MasterGameFile::unload()
	{

	}

	void MasterGameFile::store()
	{
		STONESHIP_EXCEPT(StoneshipException::UNSUPPSORTED, "MGF storage is not provided for file-based GFs");
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

		for(uint16_t i = 0; i < mDependencyCount; ++i)
		{
			if(mDependencies[i].ordinal == local)
			{
				MasterGameFile *refMgf = mManager.getLoadedMGF(mDependencies[i].filename);
				if(refMgf == nullptr)
				{
					STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, "Supposedly loaded dependency was not loaded. This probably indicates the whole program is shit.");
				}

				return refMgf->getOrdinal();
			}
		}

		STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, "Referenced ordinal not found in dependency table. This probably means a dependency was not loaded.");
	}

	RecordAccessor MasterGameFile::getRecord()
	{
		MGFDataReader ds(&mInputStream, this);

		RecordHeader recordHeader;
		ds.readStruct(recordHeader);

		return RecordAccessor(recordHeader, &mInputStream, this);
	}

	RecordAccessor MasterGameFile::getRecordByID(UID::ID id)
	{
		MGFDataReader ds(&mInputStream, this);

		ds.seek(mHeaderEndOfffset);

		for(uint32_t i = 0; i < mRecordGroupCount; ++i)
		{
			RecordHeader groupHeader;
			ds.readStruct(groupHeader);
			if(groupHeader.type != Record::TYPE_GROUP)
			{
				STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF (expected top group record, found other)");
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

		STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record not found in MGF");
	}

	RecordAccessor MasterGameFile::getRecordByTypeID(UID::ID id, Record::Type type)
	{
		if(type == Record::TYPE_LOOKUP_ALL) //this fixes a problem with uninitialized Accessors. It's ugly, but it works for now
		{
			// typeless lookup

			return getRecordByID(id);
		}

		OffsetHint *hint = _getHint(type);
		if(hint == nullptr)
		{
			// no offset hint for that type -> this file does not contain that type of record
			STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record not found in MGF (missing hint)");
		}

		// create reader and move to top group of requested type
		MGFDataReader ds(&mInputStream, this);
		ds.seek(hint->offset);

		RecordHeader groupHeader;
		ds.readStruct(groupHeader);

		if(groupHeader.type != Record::TYPE_GROUP)
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF (expected top group record, found other)");
		}

		ds.beginUnit(groupHeader.dataSize);
		while(ds.bytesRemainingInUnit())
		{
			RecordHeader recordHeader;
			ds.readStruct(recordHeader);

			if(recordHeader.id == id && recordHeader.type == type) // found it!
			{
				// leave stream pointer at data field; that's none of our business
				return RecordAccessor(recordHeader, &mInputStream, this);

			}else // this is not the record you are looking for
			{
				// skip until next record
				ds.skip(recordHeader.dataSize);
			}
		}

		STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record not found in MGF");
	}

#ifdef _DEBUG
	RecordAccessor MasterGameFile::getRecordByEditorName(const String &name, Record::Type type)
	{
		MGFDataReader ds(&mInputStream, this);

		ds.seek(mHeaderEndOfffset);

		for(uint32_t i = 0; i < mRecordGroupCount; ++i)
		{
			RecordHeader groupHeader;
			ds.readStruct(groupHeader);
			if(groupHeader.type != Record::TYPE_GROUP)
			{
				STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF (expected top group record, found other)");
			}

			ds.beginUnit(groupHeader.dataSize);
			while(ds.bytesRemainingInUnit())
			{
				RecordHeader recordHeader;
				ds.readStruct(recordHeader);

				RecordAccessor record(recordHeader, &mInputStream, this);
				try
				{
					String editorName;
					record.getReaderForSubrecord(Record::SUBTYPE_EDITOR).readSString(editorName);

					if(editorName == name)
					{
						record.rollback();
						return record;
					}
				}catch(StoneshipException &e)
				{
					if(e.getType() != StoneshipException::SUBRECORD_NOT_FOUND)
					{
						throw;
					}
				}

				record.skip(); // not a matching record. go to next one
			}
			ds.endUnit();
		}

		STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record not found in MGF");
	}
#endif

	RecordAccessor MasterGameFile::getFirstRecord(Record::Type type)
	{
		OffsetHint *hint = _getHint(type);
		if(hint == nullptr)
		{
			// no offset hint for that type -> this file does not contain that type of record
			STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record not found in MGF (missing hint)");
		}

		mInputStream.seekg(hint->offset);

		MGFDataReader ds(&mInputStream, this);

		RecordHeader groupHeader;
		ds.readStruct(groupHeader);
		if(groupHeader.type != Record::TYPE_GROUP)
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF (expected top group record, found other)");
		}

		// top group does not neccessarily start with record of associated type. Search group for first occurrence
		ds.beginUnit(groupHeader.dataSize);
		while(ds.bytesRemainingInUnit())
		{
			RecordHeader recordHeader;
			ds.readStruct(recordHeader);

			if(recordHeader.type == type)
			{
				return RecordAccessor(recordHeader, &mInputStream, this);

			}else // this is not the record you are looking for
			{
				// skip until next record
				ds.skip(recordHeader.dataSize);
			}
		}

		STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record not found in MGF");
	}

	void MasterGameFile::applyModifications(EntityBase *base)
	{
		for(uint32_t i = 0; i < mMods.size(); ++i)
		{
			if(mMods[i].uid == base->getUID())
			{
				MGFDataReader ds(&mInputStream, this);
				ds.seek(mMods[i].offset);

				RecordHeader header;
				ds.readStruct(header);

				RecordAccessor record(header, &mInputStream, this);

				base->modifyFromRecord(record);

				return; // only one Mod record per UID per MGF may be specified, so we are done here
			}
		}
	}


	MasterGameFile::OffsetHint *MasterGameFile::_getHint(Record::Type type)
	{
		static OffsetHint *cachedHint = nullptr;

		if(cachedHint == nullptr || cachedHint->type != type) // only search hint list when hint type differs from last search
		{
			// retrieve offset hint for record type
			for(uint32_t i = 0; i < mRecordGroupCount; ++i)
			{

				if(mHints[i].type == type)
				{
					cachedHint = &(mHints[i]);

					return cachedHint;
				}
			}

			return nullptr;
		}

		return cachedHint;
	}

	void MasterGameFile::_indexModifies(uint32_t recordCount)
	{
		RecordAccessor record = getRecord();

		for(uint32_t i = 0; i < recordCount; ++i)
		{
			ModHint mod;
			mod.offset = record.getOffset();
			uint8_t dummyByte;
			record.getReaderForSubrecord(Record::SUBTYPE_MODIFY_METADATA)
					.readStruct<UID>(mod.uid)
					.readUShort(mod.type)
					.readUByte(dummyByte);

			mMods.push_back(mod);

			if(i < recordCount-1) // reached last record yet?
			{
				// no -> fetch next one

				record = record.getNextRecord();
			}
		}
	}

}
