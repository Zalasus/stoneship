/*
 * GameCache.cpp
 *
 *  Created on: 7 Feb 2016
 *      Author: zalasus
 */

#include "GameCache.h"

#include <algorithm>

#include "Root.h"
#include "IEntityBase.h"
#include "MGFDataWriter.h"
#include "RecordReflector.h"
#include "RecordBuilder.h"
#include "IWorld.h"

namespace Stoneship
{

    GameCache::GameCache(Root *root)
    : mRoot(root)
    {
    }

    GameCache::~GameCache()
    {
        for(uint32_t i = 0; i < mBaseCache.size(); ++i)
        {
            delete mBaseCache[i];
        }
    }

    IEntityBase *GameCache::getBase(const UID &uid, Record::Type type)
    {
        /*auto search = mBaseCache.find(uid.toUInt64());
        if(search != mBaseCache.end())
        {
            return search->second;
        }*/

        // TODO: linear search complexity. Dunno if this could be more efficient. Maybe if we move found elements to front so often used elements are found more quickly?
        for(uint32_t i = 0; i < mBaseCache.size(); ++i)
        {
            if(mBaseCache[i]->getUID() == uid)
            {
                return mBaseCache[i];
            }
        }


        // base object is not yet cached. aquire it.

        RecordAccessor rec = mRoot->getMGFManager().getRecordByTypeID(uid, type);

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

        //apply modifications. do this only for records that were loaded from MGFs, as SGFs will never modify their own records and MGFs don't see SGF's records
        if(base->getCreatedUID().id != UID::SELF_REF_ORDINAL)
        {
            try
            {
                mRoot->getMGFManager().applyModifications(base);

            }catch(StoneshipException &e)
            {
                if(e.getType() == StoneshipException::IO_ERROR)
                {
                    STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Malformed modify record or IO error. Could not apply modifications to base.");
                }

                throw;
            }
        }


        // store new base in cache
        mBaseCache.push_back(base);

        return base;
    }

    IEntityBase *GameCache::manageBase(IEntityBase *base)
    {
        mBaseCache.push_back(base);

        return base;
    }

    IWorld *GameCache::getWorld(UID uid)
    {
        return nullptr; // TODO: implement me!
    }

    IWorld *GameCache::manageWorld(IWorld *world)
    {
        mWorldCache.push_back(world);

        return world;
    }

    uint32_t GameCache::getBaseCacheSize()
    {
        return mBaseCache.size();
    }

    void GameCache::collectGarbage()
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

    uint32_t GameCache::storeCache(MGFDataWriter &writer)
    {
        // FIXME: This combined cache breaks with our no-redundant-storage philosophy. also, building it takes a heck lot of time and is not very elegant. find better solution
        std::vector<RecordReflector*> combinedCache;

        for(uint32_t i = 0; i < mBaseCache.size(); ++i)
        {
            combinedCache.push_back(mBaseCache[i]);
        }

        for(uint32_t i = 0; i < mWorldCache.size(); ++i)
        {
            combinedCache.push_back(mWorldCache[i]);
        }


        if(combinedCache.size() == 0)
        {
            // nothing to store. skip!

            return 0;
        }


        // first, we sort the cache so we can archive proper grouping in the MGF

        // lambda for comparing record type of two Base objects
        auto compare = [](RecordReflector *a, RecordReflector *b) -> bool { return a->getRecordType() < b->getRecordType();};

        std::sort(combinedCache.begin(), combinedCache.end(), compare);


        // next, iterate over records and store them
        Record::Type lastGroup = Record::TYPE_RESERVED;
        uint32_t groupCount = 0;
        //bool hasDirtyThings = false;
        RecordBuilder groupBuilder(writer);

        for(uint32_t i = 0; i < combinedCache.size(); ++i)
        {
            RecordReflector *reflector = combinedCache[i];

            // first, check if and where we have to store this reflected
            if(reflector->getCreatedUID().ordinal != UID::SELF_REF_ORDINAL)
            {
                // this reflected was not created during runtime but loaded from a MGF. whether we need to store it or not depends on it's dirty state.

                // if reflected is dirty, we need to store a MODIFY record to allow recreation of it's current state without modifiying the MGF that created it
                // for proper grouping, this pass is delayed to a second pass, which is done by another function.
                if(reflector->isDirty())
                {
                    //hasDirtyThings = true;
                }

                // if it isn't dirty it's state can be recreated entirely by that MGF, so we don't have to store it anywhere.
                continue;
            }
            // this reflected was created during runtime or loaded from savegame (which is volatile). no MODIFY needed, regardless of dirty state
            // just store it as-is


            // do we need to start a new group?
            if(lastGroup != reflector->getRecordType())
            {
                // different record type than last reflector. have we written a header already?
                if(groupCount > 0)
                {
                    // yes -> write a footer and start new group
                    groupBuilder.endRecord();
                    groupBuilder.beginGroupRecord(reflector->getRecordType());

                }else
                {
                    // no, this is our first record group. no need to write footer. just start group right ahead
                    groupBuilder.beginGroupRecord(reflector->getRecordType());
                }

                ++groupCount;
                lastGroup = reflector->getRecordType();
            }


            RecordBuilder childBuilder = groupBuilder.createAndBeginChildBuilder(reflector->getRecordType(), 0, reflector->getCreatedUID().id);
            reflector->storeToRecord(childBuilder);
            childBuilder.endRecord();
        }

        // have we written any groups?
        if(groupCount > 0)
        {
            // yes. write final footer
            groupBuilder.endRecord();
        }

        return groupCount;
    }

}
