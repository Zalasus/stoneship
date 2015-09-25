/*
 * MasterGameFileManager.cpp
 *
 *  Created on: 27.08.2015
 *      Author: Niklas Weissner
 */

#include "MasterGameFileManager.h"

#include "MasterGameFile.h"
#include "StoneshipException.h"

namespace Stoneship
{

	MasterGameFileManager::MasterGameFileManager()
	: mLoadedGameFileCount(0)
	{
	}

	MasterGameFileManager::~MasterGameFileManager()
	{
		for(uint32_t i = 0; i < mGameFiles.size(); i++)
		{
			delete mGameFiles[i];
		}
	}

	void MasterGameFileManager::loadMGF(const String& filename)
	{
		UID::Ordinal ordinal = mGameFiles.size();
		MasterGameFile *mgf = new MasterGameFile(filename, ordinal, *this);
		mGameFiles.push_back(mgf);

		mgf->load();

		mLoadedGameFileCount++;
	}

	uint32_t MasterGameFileManager::getLoadedMGFCount() const
	{
		return mLoadedGameFileCount;
	}

	MasterGameFile *MasterGameFileManager::getLoadedMGF(UID::Ordinal ordinal)
	{
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

	MasterGameFile *MasterGameFileManager::getLoadedMGF(const String &filename)
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


	RecordAccessor MasterGameFileManager::getRecord(UID id)
	{
		if(id.ordinal >= mGameFiles.size())
		{
			throw StoneshipException("Requested record with invalid ordinal");
		}

		return mGameFiles[id.ordinal]->getRecord(id.id);
	}

	RecordAccessor MasterGameFileManager::getRecord(UID id, Record::Type type)
	{
		if(id.ordinal >= mGameFiles.size())
		{
			throw StoneshipException("Requested record with invalid ordinal");
		}

		return mGameFiles[id.ordinal]->getRecord(id.id, type);
	}

}
