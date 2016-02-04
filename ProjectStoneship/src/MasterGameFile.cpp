/*
 * MasterGameFile.cpp
 *
 *  Created on: 26.08.2015
 *      Author: Zalasus
 */


#include "MasterGameFile.h"

#include "MGFDataReader.h"
#include "MGFDataWriter.h"
#include "MGFManager.h"
#include "ResourceManager.h"
#include "Exception.h"
#include "IEntityBase.h"
#include "MGFManager.h"
#include "StoneshipDefines.h"
#include "Root.h"
#include "EntityManager.h"

namespace Stoneship
{

	MasterGameFile::MasterGameFile(const String &filename, UID::Ordinal ordinal)
	: mFilename(filename),
	  mOrdinal(ordinal),
	  mLoaded(false),
	  mFlags(0),
	  mDependencyCount(0),
	  mDependencies(nullptr),
	  mResourceCount(0),
	  mHints(nullptr),
	  mCachedHint(nullptr),
	  mRecordCount(0),
	  mRecordGroupCount(0)
	{
	    if(ordinal == UID::SELF_REF_ORDINAL)
	    {
	        // this is a SGF. that means it's dependencies are all loaded MGFs at the time of this objects instantiation

	        MGFManager *mgfm = Root::getSingleton()->getMGFManager();

	        mDependencyCount = mgfm->getLoadedMGFCount();
	        mDependencies = new Dependency[mDependencyCount];

	        for(uint32_t i = 0; i < mDependencyCount; ++i)
	        {
	            mDependencies[i].ordinal = mgfm->getLoadedMGF(i)->getOrdinal();
	            mDependencies[i].filename = mgfm->getLoadedMGF(i)->getFilename();
	        }
	    }

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
		ds >> magic;
		if(magic != 0x46474D334750524E) // "NRPG3MGF"
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Not a Master Game File: Invalid Magic ID");
		}

		String gameID;
		ds >> gameID;
		if(gameID != STONESHIP_GAMEID)
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Incompatible game (Found: '" + gameID + "')");
		}

		uint64_t timestamp;

		ds >> mFlags
		   >> mAuthor
		   >> mDescription
		   >> timestamp;

		std::time_t timestampCTime;
		timestampCTime = static_cast<time_t>(timestamp);
		mTimestamp = *std::localtime(&timestampCTime);  //FIXME: Not portable! Timestamp in MGF is always UNIX format, but localtime() may use different format. Check whether this is a real hazard and fix when neccessary

		//load and check for dependencies
		ds >> mDependencyCount;
		if(mDependencyCount > 0)
		{
			mDependencies = new Dependency[mDependencyCount];

			for(uint16_t i = 0; i < mDependencyCount; i++)
			{
				String filename;
				ds >> filename;
				if(Root::getSingleton()->getMGFManager()->getLoadedMGF(filename) == nullptr)
				{
					STONESHIP_EXCEPT(StoneshipException::DEPENDENCY_NOT_MET, "Dependency '" + filename + "' was not loaded before depending MGF");
				}

				mDependencies[i].filename = filename;
				ds >> mDependencies[i].ordinal;
			}
		}


		//define resource paths
		ds >> mResourceCount;
		for(uint16_t i = 0; i < mResourceCount; i++)
		{
			uint8_t resType;
			uint8_t resPrio;
			String resPath;

			ds >> resType
			   >> resPrio
			   >> resPath;

			switch(resType)
			{
			case RES_TYPE_FS:
				Root::getSingleton()->getResourceManager()->addResourcePath(resPath, ResourceManager::PATH_FILESYSTEM, mOrdinal);
				break;

			case RES_TYPE_ZIP:
				Root::getSingleton()->getResourceManager()->addResourcePath(resPath, ResourceManager::PATH_ZIP_FILE, mOrdinal);
				break;

			case RES_TYPE_GZIP:
				Root::getSingleton()->getResourceManager()->addResourcePath(resPath, ResourceManager::PATH_GZIP_FILE, mOrdinal);
				break;

			case RES_TYPE_SINGLE:
			default:
				STONESHIP_EXCEPT(StoneshipException::RESOURCE_ERROR, "Unknown or unsupported type given for resource " + resPath);
			}
		}


		//load records
		ds >> mRecordGroupCount;

		mHeaderEndOfffset = ds.tell(); //store this so we may navigate quickly to start of records

		//skip through the file and index every top record group
		mHints = new OffsetHint[mRecordGroupCount];
		for(uint32_t i = 0; i < mRecordGroupCount; i++)
		{
			std::streampos offset = ds.tell();

			RecordHeader groupHeader;
			ds >> groupHeader;

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

	void MasterGameFile::store()
	{
	    // ATTENTION! This is only a reference implementation. The actual implementation should consider one read only and one write only class

	    if(mOrdinal != UID::SELF_REF_ORDINAL)
	    {
	        STONESHIP_EXCEPT(StoneshipException::UNSUPPSORTED, "Tried to store read-only MGF '" + mFilename + "'");
	    }


	    std::ofstream out(mFilename.c_str(), std::ios::out | std::ios::binary);

	    if(!out.good())
	    {
	        STONESHIP_EXCEPT(StoneshipException::IO_ERROR, "Could not open MGF '" + mFilename + "' for write access");
	    }


	    MGFDataWriter writer(&out);

	    // first, write MGF header
	    writer << static_cast<uint64_t>(0x46474D334750524E) // magic ID "NRPG3MGF"
               << String(STONESHIP_GAMEID)                  // game Id
               << mFlags
               << mAuthor
               << mDescription
               << static_cast<uint64_t>(std::mktime(&mTimestamp)); // convert time structure back to epoch format and ensure 64 bit width FIXME: Not portable! mktime does not neccessarily produce epoch times on every platform


	    writer << mDependencyCount;
	    for(uint32_t i = 0; i < mDependencyCount; ++i)
	    {
	        writer << mDependencies->filename
	               << mDependencies->ordinal;
	    }

	    writer << mResourceCount;
	    for(uint32_t i = 0 ; i < mResourceCount; ++i)
	    {
	        // TODO: Write ressources here
	        writer << static_cast<uint8_t>(0xCA)
	               << static_cast<uint8_t>(0xFE)
	               << String("Did you know that with the new MGF version all Strings may contain up to 4GiB of characters? Awesome! Like anybody will ever need that");
	    }

	    writer << static_cast<uint32_t>(0xCAFEBABE); // top group count


	    // done writing header. now we need to write the top groups
	    Root::getSingleton()->getEntityManager()->storeCache(writer); // EntityManager gets to store everything it has cached
	    Root::getSingleton()->getEntityManager()->storeCacheMods(writer); // append MODIFY top group


	    writer << static_cast<uint8_t>(0xF0); // end marker (legacy)
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

	const std::tm *MasterGameFile::getTimestamp() const
	{
		return &mTimestamp; //return a pointer here because it's fucking c goddammit
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
				MasterGameFile *refMgf = Root::getSingleton()->getMGFManager()->getLoadedMGF(mDependencies[i].filename);
				if(refMgf == nullptr)
				{
					STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, "Supposedly loaded dependency was not loaded. This probably is a bug.");
				}

				return refMgf->getOrdinal();
			}
		}

		STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, "Referenced ordinal not found in dependency table. This probably means a dependency of one or more MGFs was not loaded or unloaded.");
	}

	//TODO: Typeless lookups are inefficient atm. Implement more dynamic matching to speed things up a bit
	RecordAccessor MasterGameFile::getRecordByID(UID::ID id)
	{
		MGFDataReader ds(&mInputStream, this);

		ds.seek(mHeaderEndOfffset);

		for(uint32_t i = 0; i < mRecordGroupCount; ++i)
		{
			RecordHeader groupHeader;
			ds >> groupHeader;
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
				ds >> recordHeader;

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
		ds >> groupHeader;

		if(groupHeader.type != Record::TYPE_GROUP)
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF (expected top group record, found other)");
		}

		ds.beginUnit(groupHeader.dataSize);
		while(ds.bytesRemainingInUnit())
		{
			RecordHeader recordHeader;
			ds >> recordHeader;

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

	RecordAccessor MasterGameFile::getRecordByEditorName(const String &name, Record::Type type)
	{
		MGFDataReader ds(&mInputStream, this);

		ds.seek(mHeaderEndOfffset);

		for(uint32_t i = 0; i < mRecordGroupCount; ++i)
		{
			RecordHeader groupHeader;
			ds >> groupHeader;
			if(groupHeader.type != Record::TYPE_GROUP)
			{
				STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF (expected top group record, found other)");
			}

			ds.beginUnit(groupHeader.dataSize);
			while(ds.bytesRemainingInUnit())
			{
				RecordHeader recordHeader;
				ds >> recordHeader;

				RecordAccessor record(recordHeader, &mInputStream, this);
				try
				{
					String editorName;
					record.getReaderForSubrecord(Record::SUBTYPE_EDITOR) >> editorName;

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
		ds >> groupHeader;
		if(groupHeader.type != Record::TYPE_GROUP)
		{
			STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF (expected top group record, found other)");
		}

		// top group does not neccessarily start with record of associated type. Search group for first occurrence
		ds.beginUnit(groupHeader.dataSize);
		while(ds.bytesRemainingInUnit())
		{
			RecordHeader recordHeader;
			ds >> recordHeader;

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

	void MasterGameFile::applyModifications(RecordReflector *reflector)
	{
		for(uint32_t i = 0; i < mMods.size(); ++i)
		{
			if(mMods[i].uid == reflector->getCreatedUID())
			{
				MGFDataReader ds(&mInputStream, this);
				ds.seek(mMods[i].offset);

				RecordHeader header;
				ds >> header;

				RecordAccessor record(header, &mInputStream, this);

				reflector->loadFromModifyRecord(record);

				return; // only one Mod record per UID per MGF may be specified, so we are done here
			}
		}
	}


	MasterGameFile::OffsetHint *MasterGameFile::_getHint(Record::Type type)
	{

		if(mCachedHint == nullptr || mCachedHint->type != type) // only search hint list when hint type differs from last search
		{
			// retrieve offset hint for record type
			for(uint32_t i = 0; i < mRecordGroupCount; ++i)
			{

				if(mHints[i].type == type)
				{
					mCachedHint = &(mHints[i]);

					return mCachedHint;
				}
			}

			return nullptr;
		}

		return mCachedHint;
	}

	void MasterGameFile::_indexModifies(uint32_t recordCount)
	{
	    MGFDataReader ds(&mInputStream, this);

	    RecordHeader header;
	    ds >> header;

	    RecordAccessor record(header, &mInputStream, this);

		for(uint32_t i = 0; i < recordCount; ++i)
		{
			ModHint mod;
			mod.offset = record.getOffset();
			record.getReaderForSubrecord(Record::SUBTYPE_MODIFY_METADATA)
					>> mod.uid
					>> mod.type
					>> mod.modType;

			mMods.push_back(mod);

			if(i < recordCount-1) // reached last record yet?
			{
				// no -> fetch next one

				record = record.getNextRecord();
			}
		}
	}

}
