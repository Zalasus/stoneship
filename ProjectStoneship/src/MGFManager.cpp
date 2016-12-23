/*
 * MGFManager.cpp
 *
 *  Created on: 27.08.2015
 *      Author: Niklas Weissner
 */

#include <IEntity.h>
#include "Exception.h"
#include "MGFManager.h"
#include "MasterGameFile.h"
#include "Logger.h"

namespace Stoneship
{

	MGFManager::MGFManager(ResourceManager *resourceManager)
	: mResourceManager(resourceManager)
    , mCurrentSaveFile(nullptr)
    , mLoadedGameFileCount(0)
	{
	}

	MGFManager::~MGFManager()
	{
		for(uint32_t i = 0; i < mGameFiles.size(); i++)
		{
			delete mGameFiles[i];
		}

		if(mCurrentSaveFile != nullptr)
		{
			delete mCurrentSaveFile;
		}
	}

	void MGFManager::loadMGF(const String& filename)
	{
	    if(getLoadedMGF(filename) != nullptr)
	    {
	        Logger::warn("Tried to load MGF '" + filename + "' when it was already loaded. Skipping");

	        return;
	    }

		UID::Ordinal ordinal = mGameFiles.size();
		MasterGameFile *mgf = new MasterGameFile(filename, ordinal, this);
		mGameFiles.push_back(mgf);

		mgf->load();

		mLoadedGameFileCount++;
	}

	void MGFManager::loadSGF(const String &filename)
	{
	    if(mCurrentSaveFile != nullptr)
	    {
	        STONESHIP_EXCEPT(StoneshipException::INVALID_STATE, "Tried to load SGF when one was still loaded");
	    }

	    mCurrentSaveFile = new MasterGameFile(filename, UID::SELF_REF_ORDINAL, this);
	    mCurrentSaveFile->load();
	}

	void MGFManager::storeSGF(const String &filename, GameCache *gameCache)
	{
	    Logger::info("Saving SGF '" + filename + "'...");

	    if(mCurrentSaveFile != nullptr)
	    {
	        delete mCurrentSaveFile;
	    }

	    mCurrentSaveFile = new MasterGameFile(filename, UID::SELF_REF_ORDINAL, this);
	    mCurrentSaveFile->initCreated();

	    mCurrentSaveFile->store(gameCache);
	}

	uint32_t MGFManager::getLoadedMGFCount() const
	{
		return mLoadedGameFileCount;
	}

	MasterGameFile *MGFManager::getLoadedMGFByIndex(uint32_t i)
	{
		if(i >= mGameFiles.size())
		{
			return nullptr;
		}

		if(!mGameFiles[i]->isLoaded())
		{
			return nullptr;
		}

		return mGameFiles[i];
	}

	MasterGameFile *MGFManager::getGameFileByOrdinal(UID::Ordinal ordinal)
	{
	    if(ordinal == UID::SELF_REF_ORDINAL)
	    {
	        return mCurrentSaveFile;
	    }

	    return getLoadedMGFByIndex(ordinal);
	}

	MasterGameFile *MGFManager::getLoadedMGF(const String &filename)
	{
	    /*if(mCurrentSaveFile != nullptr && mCurrentSaveFile->getFilename() == filename)
	    {
	        return mCurrentSaveFile;
	    }*/

		for(uint32_t i = 0; i < mGameFiles.size(); i++)
		{
			if(mGameFiles[i]->getFilename() == filename && mGameFiles[i]->isLoaded())
			{
				return mGameFiles[i];
			}
		}

		return nullptr;
	}

	UID MGFManager::getNewUID(UID::Ordinal ordinal)
	{
	    static UID::ID lastID = 0xF000000; //TODO: implement proper ID generation

	    return UID(ordinal, lastID++);
	}

	//TODO: These functions look almost identical. I don't like that
	RecordAccessor MGFManager::getRecordByID(UID id)
	{
		MasterGameFile *mgf = getGameFileByOrdinal(id.ordinal);

		if(mgf == nullptr)
		{
		    STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, String("Requested record with invalid ordinal. MGF ") + id.ordinal + " not loaded (UID: " + id.toString() + ")");
		}

		return mgf->getRecordByID(id.id);
	}

	RecordAccessor MGFManager::getRecordByTypeID(UID id, Record::Type type)
	{
		MasterGameFile *mgf = getGameFileByOrdinal(id.ordinal);

		if(mgf == nullptr)
		{
			STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, String("Requested record with invalid ordinal. MGF ") + id.ordinal + " not loaded (UID: " + id.toString() + ")");
		}

		return mgf->getRecordByTypeID(id.id, type);
	}

	RecordAccessor MGFManager::getRecordByEditorName(const String &name)
	{
		for(uint16_t i = 0; i < mLoadedGameFileCount; ++i)
		{
			try
			{
				RecordAccessor record = getLoadedMGFByIndex(i)->getRecordByEditorName(name);
				return record;

			}catch(StoneshipException &e)
			{
				if(e.getType() != StoneshipException::RECORD_NOT_FOUND)
				{
					throw;
				}
			}
		}

		STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Record with editor name '" + name + "' not found in loaded MGFs");
	}

	void MGFManager::applyModifications(RecordReflector *loadable, GameCache *gameCache)
	{
		for(uint32_t i = 0; i < mLoadedGameFileCount; ++i) // modifications are applied incrementally. not sure if this is the most efficient way, but it should work
		{
		    if(mGameFiles[i]->getOrdinal() == loadable->getCreatedUID().ordinal)
			{
		        // MGFs do not contain Modify records for their own records. no need to iterate the table
		        continue;
			}

		    mGameFiles[i]->applyModifications(loadable, gameCache);
		}

		if(mCurrentSaveFile != nullptr && mCurrentSaveFile->getOrdinal() != loadable->getCreatedUID().ordinal)
		{
			mCurrentSaveFile->applyModifications(loadable, gameCache);
		}
	}

}
