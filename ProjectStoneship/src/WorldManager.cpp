/*
 * WorldManager.cpp
 *
 *  Created on: 22.09.2015
 *      Author: Zalasus
 */

#include "WorldManager.h"

#include <iostream>
#include <algorithm>

#include "IEntity.h"
#include "WorldManager.h"
#include "IWorld.h"
#include "WorldDungeon.h"
#include "WorldOutdoor.h"
#include "Exception.h"
#include "MasterGameFile.h"
#include "Root.h"
#include "MGFManager.h"
#include "MGFDataWriter.h"
#include "EntityManager.h"
#include "RecordBuilder.h"

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
	}

	IWorld *WorldManager::getCurrentWorld()
	{
	    return mCurrentWorld;
	}

	uint32_t WorldManager::storeWorldCache(MGFDataWriter &writer)
	{
	    if(mWorldCache.size() == 0)
	    {
	        // nothing to be stored. abort
	        return 0;
	    }


	    // first, we sort the cache so we can archive proper grouping in the MGF

        // lambda for comparing world types (just two ATM, but might get more in the futurw
        auto compare = [](IWorld *a, IWorld *b) -> bool { return a->getRecordType() < b->getRecordType();};

        std::sort(mWorldCache.begin(), mWorldCache.end(), compare);


        Record::Type lastGroup = Record::TYPE_RESERVED;
        uint32_t groupCount = 0;
        RecordBuilder groupBuilder(writer);

	    for(uint32_t i = 0; i < mWorldCache.size(); ++i)
	    {
	        IWorld *world = mWorldCache[i];

	        if(world->getCreatedUID().ordinal != UID::SELF_REF_ORDINAL)
	        {
	            // this world was not created during runtime. no storage needed
	            //  (as long as it's not dirty, but that is dealt with in the storeWorldCacheMods() function
	            continue;
	        }

	        // do we need to start a new group?
            if(lastGroup != world->getRecordType())
            {
                // different record type than last world. have we written a header already?
                if(groupCount > 0)
                {
                    // yes -> write a footer and start new group
                    groupBuilder.endRecord();
                    groupBuilder.beginGroupRecord(world->getRecordType());

                }else
                {
                    // no, this is our first record group. no need to write footer. just start group right ahead
                    groupBuilder.beginGroupRecord(world->getRecordType());
                }

                ++groupCount;
                lastGroup = world->getRecordType();
            }


            RecordBuilder childBuilder = groupBuilder.createAndBeginChildBuilder(world->getRecordType(), 0, world->getUID().id);
            world->storeToRecord(childBuilder);
            childBuilder.endRecord();
	    }

	    return 0;
	}

}


