/*
 * EntityManager.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "EntityManager.h"

#include <algorithm>

#include "IEntityBase.h"
#include "Exception.h"
#include "MGFManager.h"
#include "Root.h"
#include "RecordAccessor.h"
#include "RecordBuilder.h"

namespace Stoneship
{

	EntityManager::EntityManager(Root *root)
	: mRoot(root)
	{
	}

	EntityManager::~EntityManager()
	{
		for(uint32_t i = 0; i < mBaseCache.size(); ++i)
		{
			delete mBaseCache[i];
		}
	}

	IEntityBase *EntityManager::getBase(const UID &uid, Record::Type type)
	{
		/*auto search = mBaseCache.find(uid.toUInt64());
		if(search != mBaseCache.end())
		{
			return search->second;
		}*/

		//TODO: Make search for cached elements more efficient, but beware memory leak problem of unordered_map
		for(uint32_t i = 0; i < mBaseCache.size(); ++i)
		{
			if(mBaseCache[i]->getUID() == uid)
			{
				return mBaseCache[i];
			}
		}


		// base object is not yet cached. aquire it.

		RecordAccessor rec = mRoot->getMGFManager()->getRecordByTypeID(uid, type);

		EntityBaseFactory *factory = EntityBaseFactory::getFactoryForRecordType(rec.getHeader().type);
		if(factory == nullptr)
		{
			STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, String("Record type ") + rec.getHeader().type + " does not match any registered Entity Base types.");
		}

		IEntityBase *base = factory->createEntityBase(uid);

		try
		{
			base->loadFromRecord(rec);

		}catch(StoneshipException &e)
		{
			if(e.getType() == StoneshipException::IO_ERROR)
			{
				STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Malformed record or IO error. Could not load entity base.");
			}

			throw;
		}

		//apply modifications. do this for every new record for now. we might think of situations when we don't have to do this in the future
		try
		{
			mRoot->getMGFManager()->applyModifications(base);

		}catch(StoneshipException &e)
		{
			if(e.getType() == StoneshipException::IO_ERROR)
			{
				STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Malformed modify record or IO error. Could not apply modifications to base.");
			}

			throw;
		}


		// store new base in cache
		mBaseCache.push_back(base);

		return base;
	}

	IEntityBase *EntityManager::manageBase(IEntityBase *base)
	{
	    mBaseCache.push_back(base);

	    return base;
	}

	uint32_t EntityManager::getBaseCacheSize()
	{
		return mBaseCache.size();
	}

	void EntityManager::collectGarbage()
	{
		auto it = mBaseCache.begin();
		while(it != mBaseCache.end())
		{
		    IEntityBase *base = (*it);

		    // don't delete a base that's still used, is dirty or was created ingame
			if((base->getUserCount() == 0) && !base->isDirty() && (base->getCreatedUID().ordinal == UID::SELF_REF_ORDINAL))
			{
				delete base;

				it = mBaseCache.erase(it);

			}else
			{
				++it;
			}
		}
	}

	void EntityManager::storeCache(MGFDataWriter &writer)
	{
	    // first, we sort the cache so we can archive proper grouping in the MGF

	    // lambda for comparing record type of two Base objects
	    auto compare = [](IEntityBase *a, IEntityBase *b) -> bool { return a->getRecordType() < b->getRecordType();};

	    std::sort(mBaseCache.begin(), mBaseCache.end(), compare);


	    // next, iterate over records and store them
	    Record::Type lastGroup = Record::TYPE_RESERVED;
	    uint32_t groupCount = 0;
	    bool hasDirtyThings = false;
	    RecordBuilder groupBuilder(writer, Record::TYPE_RESERVED);

	    for(uint32_t i = 0; i < mBaseCache.size(); ++i)
        {
            IEntityBase *base = mBaseCache[i];

            // first, check if and where we have to store this Base
            if(base->getCreatedUID().ordinal != UID::SELF_REF_ORDINAL)
            {
                // this base was not created during runtime but loaded from a MGF. whether we need to store it or not depends on it's dirty state.

                // if base is dirty, we need to store a MODIFY record to allow recreation of it's current state without modifiying the MGF that created it
                // for proper grouping, this pass is delayed to a second pass, which is done by another function.
                if(base->isDirty())
                {
                    hasDirtyThings = true;
                }

                // if it isn't dirty it's state can be recreated entirely by that MGF, so we don't have to store it anywhere.
                continue;
            }
            // this base was created during runtime or loaded from savegame (which is volatile). no MODIFY needed, regardless of dirty state
            // just store it as-is


            // do we need to start a new group?
            if(lastGroup != base->getRecordType())
            {
                // different record type than last base. have we written a header already?
                if(groupCount > 0)
                {
                    // yes -> write a footer and start new group using that handy function
                    groupBuilder.endRecordBeginNew(Record::TYPE_GROUP, 0, UID::NO_ID, base->getRecordType());

                }else
                {
                    // no, this is our first record group. re-initialize builder accordingly and write header
                   // groupBuilder = RecordBuilder(writer, Record::TYPE_GROUP, 0, UID::NO_ID, base->getRecordType());
                    groupBuilder.beginRecord();
                }

                ++groupCount;
                lastGroup = base->getRecordType();
            }


            RecordBuilder childBuilder = groupBuilder.createChildBuilder(base->getRecordType(), 0, base->getUID().id);
            base->storeToRecord(childBuilder);
            childBuilder.endRecord();
        }

	    // have we written any groups?
	    if(groupCount > 0)
	    {
	        // yes. write final footer
	        groupBuilder.endRecord();
	    }
	}
}
