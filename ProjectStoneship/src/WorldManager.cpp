/*
 * WorldManager.cpp
 *
 *  Created on: 22.09.2015
 *      Author: Zalasus
 */

#include "WorldManager.h"

#include <iostream>

#include "IEntity.h"
#include "WorldManager.h"
#include "IWorld.h"
#include "WorldDungeon.h"
#include "WorldOutdoor.h"
#include "Exception.h"
#include "MasterGameFile.h"
#include "Root.h"
#include "MGFManager.h"
#include "EntityManager.h"

namespace Stoneship
{

	WorldManager::WorldManager(Root *root)
	: mRoot(root),
	  mCurrentWorld(nullptr)
	{
	}

	WorldManager::~WorldManager()
	{
	    if(mCurrentWorld != nullptr)
	    {
	        delete mCurrentWorld;
	    }
	}

	void WorldManager::enterWorld(UID worldUID)
	{
	    if(mCurrentWorld != nullptr)
	    {
	        delete mCurrentWorld;
	        mCurrentWorld = nullptr;
	    }

	    RecordAccessor worldRecord = mRoot->getMGFManager()->getRecordByID(worldUID); //TODO: We only have two possible types this lookup should yield. Make it typed

		if(worldRecord.getHeader().type == Record::TYPE_DUNGEON)
		{
		    mCurrentWorld = new WorldDungeon(worldUID);

		}else if(worldRecord.getHeader().type == Record::TYPE_OUTDOOR)
		{

		    mCurrentWorld = new WorldOutdoor(worldUID);

		}else
		{
		    STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, worldUID.toString() + " is not a world record");
		}

		mCurrentWorld->loadFromRecord(worldRecord);

		mRoot->getEntityManager()->collectGarbage(); //remove all bases that are not used atm

		mRoot->getRenderManager()->rebuildSceneGraph();
	}

	IWorld *WorldManager::getCurrentWorld()
	{
	    return mCurrentWorld;
	}

}


