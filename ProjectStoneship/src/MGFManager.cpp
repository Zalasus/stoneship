/*
 * MGFManager.cpp
 *
 *  Created on: 27.08.2015
 *      Author: Niklas Weissner
 */

#include "MGFManager.h"
#include "MasterGameFile.h"
#include "StoneshipException.h"
#include "Entity.h"

namespace Stoneship
{

	MGFManager::MGFManager(Root *root)
	: mRoot(root),
	  mCurrentSaveFile(nullptr),
	  mLoadedGameFileCount(0)
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
		UID::Ordinal ordinal = mGameFiles.size();
		MasterGameFile *mgf = new MasterGameFile(filename, ordinal, this, mRoot->getResourceManager());
		mGameFiles.push_back(mgf);

		mgf->load();

		mLoadedGameFileCount++;
	}

	uint32_t MGFManager::getLoadedMGFCount() const
	{
		return mLoadedGameFileCount;
	}

	MasterGameFile *MGFManager::getLoadedMGF(UID::Ordinal ordinal)
	{
		if(ordinal == UID::SELF_REF_ORDINAL)
		{
			return mCurrentSaveFile;
		}

		if(ordinal >= mGameFiles.size())
		{
			return nullptr;
		}

		if(!mGameFiles[ordinal]->isLoaded())
		{
			return nullptr;
		}

		return mGameFiles[ordinal];
	}

	MasterGameFile *MGFManager::getLoadedMGF(const String &filename)
	{
		for(uint32_t i = 0; i < mGameFiles.size(); i++)
		{
			if(mGameFiles[i]->getFilename() == filename && mGameFiles[i]->isLoaded())
			{
				return mGameFiles[i];
			}
		}

		return nullptr;
	}

	void MGFManager::loadSGF(const String &savename)
	{
		if(mCurrentSaveFile != nullptr)
		{
			mCurrentSaveFile->unload();

			delete mCurrentSaveFile;

			mCurrentSaveFile = nullptr;
		}

		mCurrentSaveFile = new MasterGameFile(savename, UID::SELF_REF_ORDINAL, this, mRoot->getResourceManager());
		mCurrentSaveFile->load();
	}


	//TODO: These functions look almost identical. I don't like that
	RecordAccessor MGFManager::getRecordByID(UID id)
	{
		MasterGameFile *mgf = getLoadedMGF(id.ordinal);

		if(mgf == nullptr)
		{
			STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, "Requested record with invalid ordinal");
		}

		return mgf->getRecordByID(id.id);
	}

	RecordAccessor MGFManager::getRecordByTypeID(UID id, Record::Type type)
	{
		MasterGameFile *mgf = getLoadedMGF(id.ordinal);

		if(mgf == nullptr)
		{
			STONESHIP_EXCEPT(StoneshipException::MGF_NOT_FOUND, "Requested record with invalid ordinal");
		}

		return mgf->getRecordByTypeID(id.id, type);
	}

#ifdef _DEBUG
	RecordAccessor MGFManager::getRecordByEditorName(const String &name, Record::Type type)
	{
		for(uint16_t i = 0; i < mLoadedGameFileCount; ++i)
		{
			try
			{
				RecordAccessor record = mGameFiles[i]->getRecordByEditorName(name, type);
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
#endif

	void MGFManager::applyModifications(EntityBase *base)
	{
		for(uint32_t i = 0; i < mLoadedGameFileCount; ++i) // modifications are applied incrementally. not sure if this is the most efficient way, but it should work
		{
			mGameFiles[i]->applyModifications(base);
		}

		if(mCurrentSaveFile != nullptr)
		{
			mCurrentSaveFile->applyModifications(base);
		}
	}

}
