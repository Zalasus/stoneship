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
#include "GameCache.h"
#include "Logger.h"

namespace Stoneship
{

	MasterGameFile::MasterGameFile(const String &filename, UID::Ordinal ordinal, MGFManager *mgfManager)
	: mFilename(filename)
    , mOrdinal(ordinal)
    , mMGFManager(mgfManager)
    , mLoaded(false)
    , mFlags(0)
    , mAuthor("Zalasus")
    , mDescription("Just a test MGF")
    , mDependencies()
    , mResources()
    , mHints()
    , mCachedHint(nullptr)
    , mRecordGroupCount(0)
	{
	}

	MasterGameFile::~MasterGameFile()
	{
	    if(mInputStream.is_open())
	    {
	        mInputStream.close();
	    }
	}

	void MasterGameFile::load(bool ignoreDependencies)
	{
	    if(mLoaded)
	    {
	        STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to load already loaded or initialized game file.");
	    }


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
		uint16_t dependencyCount;
		ds >> dependencyCount;
		if(dependencyCount > 0)
		{
			mDependencies.allocate(dependencyCount);

			for(uint16_t i = 0; i < dependencyCount; i++)
			{
				String filename;
				ds >> filename;
				if(mMGFManager->getLoadedMGF(filename) == nullptr && !ignoreDependencies)
				{
					STONESHIP_EXCEPT(StoneshipException::DEPENDENCY_NOT_MET, "Dependency '" + filename + "' was not loaded before depending MGF");
				}

				mDependencies[i].filename = filename;
				ds >> mDependencies[i].ordinal;
			}
		}


		//define resource paths
		uint16_t resourceCount;
		ds >> resourceCount;
		mResources.allocate(resourceCount);
		for(uint16_t i = 0; i < resourceCount; i++)
		{
			uint8_t resType;
			uint8_t resDummy;
			String resPath;

			ds >> resType
			   >> resDummy
			   >> resPath;

			mResources[i].path = resPath;

			switch(resType)
			{
			case RES_TYPE_FS:
			    mResources[i].type = ResourceManager::PATH_FILESYSTEM;
				mMGFManager->mResourceManager->addResourcePath(resPath, ResourceManager::PATH_FILESYSTEM, mOrdinal);
				break;

			case RES_TYPE_ZIP:
			    mResources[i].type = ResourceManager::PATH_ZIP_FILE;
				mMGFManager->mResourceManager->addResourcePath(resPath, ResourceManager::PATH_ZIP_FILE, mOrdinal);
				break;

			case RES_TYPE_GZIP:
			    mResources[i].type = ResourceManager::PATH_GZIP_FILE;
				mMGFManager->mResourceManager->addResourcePath(resPath, ResourceManager::PATH_GZIP_FILE, mOrdinal);
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
        mHints.allocate(mRecordGroupCount);
        for(uint32_t i = 0; i < mRecordGroupCount; i++)
        {
            std::streampos offset = ds.tell();

            RecordHeader groupHeader;
            try
            {
                ds >> groupHeader;

            }catch(StoneshipException &e)
            {
                if(e.getType() == StoneshipException::IO_ERROR)
                {
                    STONESHIP_EXCEPT(StoneshipException::IO_ERROR, "Error while scanning top groups. IO error while loading next group header."
                            "This is most likely caused by an invalid group count field in the MGF header. Original error: "
                            + e.getMessage());
                }

                throw;
            }

            if(groupHeader.type != Record::TYPE_GROUP)
            {
                STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, String("Corrupted MGF found during scanning (expected Group record, found ") + groupHeader.type);
            }

            if(!(groupHeader.flags & RecordHeader::FLAG_TOP_GROUP))
            {
                STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF found during scanning (scanned Group was not a top group)");
            }

            Logger::debug(String("Found group ") + groupHeader.groupType + " at " + uint32_t(offset) + " containing " + groupHeader.recordCount + " records");

            mHints[i].offset = offset;
            mHints[i].type = groupHeader.groupType;
            mHints[i].recordCount = groupHeader.recordCount;

            //there are some records which we want to index one by one like Modify or load right at startup. check if we have on of those on our hands
            EntityBaseFactory *factory = EntityBaseFactory::getFactoryForRecordType(groupHeader.type);
            if((factory != nullptr && factory->isPreloaded()))
            {
                // this is an entity base group which must be preloaded
                STONESHIP_EXCEPT(StoneshipException::UNSUPPSORTED, "Preloaded bases unsupported ATM");

            }else if(mHints[i].type == Record::TYPE_MODIFY)
            {

                _indexModifies(mHints[i].recordCount);

            }else
            {
                //no special group here. just skip to the next one
                ds.skip(groupHeader.dataSize);
            }
        }
        
        mEndMarkerOffset = mInputStream.tellg();

		mLoaded = true;

		if(mOrdinal == UID::SELF_REF_ORDINAL)
		{
		    // this is an SGF. we don't need to keep open the file stream

		    mInputStream.close();
		}
	}

	void MasterGameFile::store(GameCache *gameCache)
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


	    uint16_t dependencyCount = static_cast<uint16_t>(mDependencies.size());
	    writer << dependencyCount;
	    for(uint16_t i = 0; i < dependencyCount; ++i)
	    {
	        writer << mDependencies[i].filename
	               << mDependencies[i].ordinal;
	    }

	    uint16_t resourceCount = static_cast<uint16_t>(mResources.size());
	    writer << resourceCount;
	    for(uint16_t i = 0 ; i < resourceCount; ++i)
	    {
	        uint8_t resType;

	        switch(mResources[i].type)
            {
            case ResourceManager::PATH_FILESYSTEM:
                resType = RES_TYPE_FS;
                break;

            case ResourceManager::PATH_ZIP_FILE:
                resType = RES_TYPE_ZIP;
                break;

            case ResourceManager::PATH_GZIP_FILE:
                resType = RES_TYPE_GZIP;
                break;

            default:
                STONESHIP_EXCEPT(StoneshipException::RESOURCE_ERROR, "Unknown or unsupported type given for resource " + mResources[i].path);
            }

	        writer << resType
	               << static_cast<uint8_t>('Z') // dummy byte
	               << mResources[i].path;
	    }

	    std::streampos topGroupCountOff = writer.tell();
	    writer << static_cast<uint32_t>(0xCAFEBABE); // top group count (overwritten later)

	    // done writing header. now we need to write the top groups
	    mRecordGroupCount = 0;
	    mRecordGroupCount += gameCache->storeCache(writer, true); // GameCache gets to store everything it has cached
	    mRecordGroupCount += gameCache->storeCacheMods(writer); // append MODIFY top group

	    std::streampos prev = writer.tell();
	    writer.seek(topGroupCountOff);
	    writer << mRecordGroupCount;
	    writer.seek(prev);

	    Logger::debug(String("Stored ") + mRecordGroupCount + " record groups");

	    writer << static_cast<uint8_t>(0xF0); // end marker (legacy)

	    out.close();
	}

	void MasterGameFile::initCreated()
	{
	    if(mLoaded || mOrdinal != UID::SELF_REF_ORDINAL)
        {
	        STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to init loaded SGF, MGF or reinitialized SGF");
        }

        mDependencies.allocate(mMGFManager->getLoadedMGFCount());

        for(uint32_t i = 0; i < mDependencies.size(); ++i)
        {
            if(mMGFManager->getLoadedMGFByIndex(i)->getOrdinal() == getOrdinal())
            {
                // no self dependence please
                //continue; This would lead to uninitialized deps. since MGF access methods of MGFManager exclude SGFs ATM, we can consider this not needed for now
            }

            mDependencies[i].ordinal = mMGFManager->getLoadedMGFByIndex(i)->getOrdinal();
            mDependencies[i].filename = mMGFManager->getLoadedMGFByIndex(i)->getFilename();
        }

        // initialize timestamp with current time
        time_t rawtime;
        std::time(&rawtime);
        mTimestamp = *std::localtime(&rawtime);

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
		return mDependencies.size();
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
		return mResources.size();
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

		for(uint16_t i = 0; i < mDependencies.size(); ++i)
		{
			if(mDependencies[i].ordinal == local)
			{
				MasterGameFile *refMgf = mMGFManager->getLoadedMGF(mDependencies[i].filename);
				if(refMgf == nullptr)
				{
					STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, "Supposedly loaded dependency was not loaded. This probably is a bug.");
				}

				return refMgf->getOrdinal();
			}
		}

		STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, "Referenced ordinal not found in dependency table. This probably means a dependency of one or more MGFs was not loaded or wrongly unloaded.");
	}

	//TODO: Typeless lookups are inefficient atm. Implement more dynamic matching to speed things up a bit
	RecordAccessor MasterGameFile::getRecordByID(UID::ID id)
	{
	    Logger::debug("Typeless lookup. Slooooowwww!!!");

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
					return RecordAccessor(recordHeader, &mInputStream, mInputStream.tellg(), this);

				}else //this is not the record you are looking for
				{
					//skip until next record
					ds.skip(recordHeader.dataSize);
				}
			}
			ds.endUnit();
		}

		STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record " + UID(mOrdinal, id).toString() + " not found in MGF '" + mFilename + "'");
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
				return RecordAccessor(recordHeader, &mInputStream, mInputStream.tellg(), this);

			}else // this is not the record you are looking for
			{
				// skip until next record
				ds.skip(recordHeader.dataSize);
			}
		}

		STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record " + UID(mOrdinal, id).toString() + " of type " + type + " not found in MGF '" + mFilename + "'");
	}

	RecordAccessor MasterGameFile::getRecordByEditorName(const String &name)
	{
		MGFDataReader ds(&mInputStream, this);

		ds.seek(mHeaderEndOfffset);

		//TODO: searches only top groups atm. need to linearize MGF here to reach all nested groups
		for(uint32_t i = 0; i < mRecordGroupCount; ++i)
		{
			RecordHeader groupHeader;
			ds >> groupHeader;
			if(groupHeader.type != Record::TYPE_GROUP)
			{
				STONESHIP_EXCEPT(StoneshipException::DATA_FORMAT, "Corrupted MGF (expected top group record, found other)");
			}

            ds.beginUnit(groupHeader.dataSize);

			if(!(groupHeader.flags & RecordHeader::FLAG_EDATA_PRESENT))
			{
			    ds.skipToEnd(); // no edata in this group. not neccessary to search it
			}

			for(uint32_t i = 0; i < groupHeader.recordCount; ++i)
			{
				RecordHeader recordHeader;
				ds >> recordHeader;

				RecordAccessor record(recordHeader, &mInputStream, mInputStream.tellg(), this);
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

	RecordIterator MasterGameFile::getRecordIterator()
	{
        return RecordIterator(mHeaderEndOfffset, &mInputStream, this);
	}

	RecordIterator MasterGameFile::getRecordIteratorForType(Record::Type type)
	{
	    OffsetHint *hint = _getHint(type);
		if(hint == nullptr)
		{
			// no offset hint for that type -> this file does not contain that type of record
			STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, String("Record type ") + type + " not found in MGF (missing hint)");
		}
		
		return RecordIterator(hint->offset, &mInputStream, this);
	}
	
	RecordIterator MasterGameFile::getRecordEnd()
	{
	    return RecordIterator(mEndMarkerOffset, nullptr, this);
	}
	
	/*RecordAccessor MasterGameFile::getFirstRecordOfType(Record::Type type)
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
	}*/

	void MasterGameFile::applyModifications(RecordReflector *reflector, GameCache *gameCache)
	{
		for(uint32_t i = 0; i < mMods.size(); ++i)
		{
			if(mMods[i].uid == reflector->getCreatedUID())
			{
				MGFDataReader ds(&mInputStream, this);
				ds.seek(mMods[i].offset);

				RecordHeader header;
				ds >> header;

				RecordAccessor record(header, &mInputStream, mInputStream.tellg(), this);

				reflector->loadFromModifyRecord(record, gameCache);

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
	    RecordIterator it(mInputStream.tellg(), &mInputStream, this);
	    
	    for(uint32_t i = 0; i < recordCount; ++i)
        {
            ModHint mod;
			mod.offset = it->getOffset();
			it->getReaderForSubrecord(Record::SUBTYPE_MODIFY_METADATA)
					>> mod.uid
					>> mod.type
					>> mod.modType;

			mMods.push_back(mod);
            
            ++it;
        }
	}

}
