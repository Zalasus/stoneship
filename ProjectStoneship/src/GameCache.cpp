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
#include "Logger.h"

namespace Stoneship
{

    GameCache::GameCache(Root *root)
    : mRoot(root),
      mPolicy(POLICY_KEEP_NEEDED),
      mLRULimit(3)
    {
    }

    GameCache::~GameCache()
    {
        for(uint32_t i = 0; i < mBaseCache.size(); ++i)
        {
            delete mBaseCache[i];
        }
    }

    void GameCache::setCachePolicy(GameCachePolicy policy)
    {
        mPolicy = policy;
    }

    void GameCache::setLRULimit(uint32_t i)
    {
        mLRULimit = i;
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
            rec.skip();
            base->postLoad(rec, rec); // FIXME: second parameter should be the surrounding record. it's not used ATM, so this should be fine, but someday this will produce a hell lot of bugs

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
        manageBase(base);

        return base;
    }

    void GameCache::manageBase(IEntityBase *base)
    {
        mBaseCache.push_back(base);
    }

    IWorld *GameCache::getWorld(UID uid)
    {
        return nullptr; // TODO: implement me!
    }

    void GameCache::manageWorld(IWorld *world)
    {
        mWorldCache.push_back(world);
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

            // should we delete this base?
            bool del;
            switch(mPolicy)
            {
            case POLICY_KEEP_NEEDED:
                // don't delete a base that's still used, is dirty or was created ingame
                del = ((base->getUserCount() == 0) && !base->isDirty() && (base->getCreatedUID().ordinal == UID::SELF_REF_ORDINAL));
                break;

            // TODO: Implement this policy. Just do the same as for the default one for now
            case POLICY_KEEP_RECENTLY_USED:
                del = ((base->getUserCount() == 0) && !base->isDirty() && (base->getCreatedUID().ordinal == UID::SELF_REF_ORDINAL));
                break;

            case POLICY_PRELOAD_ALL:
            case POLICY_KEEP_ALL:
            default:
                del = false;
                break;
            }

            if(del)
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

        // build combined cache of all elements that need to be stored (were newly created)
        for(uint32_t i = 0; i < mBaseCache.size(); ++i)
        {
            // only transfer newly created bases
            if(mBaseCache[i]->getCreatedUID().ordinal == UID::SELF_REF_ORDINAL)
            {
                combinedCache.push_back(mBaseCache[i]);
            }
        }

        for(uint32_t i = 0; i < mWorldCache.size(); ++i)
        {
            // only transfer newly created worlds
            if(mWorldCache[i]->getCreatedUID().ordinal == UID::SELF_REF_ORDINAL)
            {
                combinedCache.push_back(mWorldCache[i]);
            }
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
        RecordBuilder groupBuilder(writer);

        for(uint32_t i = 0; i < combinedCache.size(); ++i)
        {
            RecordReflector *reflector = combinedCache[i];

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
                    Logger::debug(String("Stored record group ") + lastGroup + " containing " + groupBuilder.getChildRecordCount() + " records");
                    groupBuilder.beginGroupRecord(reflector->getRecordType());

                }else
                {
                    // no, this is our first record group. no need to write footer. just start group right ahead
                    groupBuilder.beginGroupRecord(reflector->getRecordType());
                }

                // set the top group flag
                groupBuilder.setFlags(groupBuilder.getFlags() | RecordHeader::FLAG_TOP_GROUP);

                ++groupCount;
                lastGroup = reflector->getRecordType();
            }


            RecordBuilder childBuilder = groupBuilder.createChildBuilder();
            childBuilder.beginRecord(reflector->getRecordType(), 0, reflector->getCreatedUID().id);
            reflector->storeToRecord(childBuilder);
            childBuilder.endRecord();
            reflector->postStore(childBuilder, groupBuilder);
        }

        // have we written any groups?
        if(groupCount > 0)
        {
            // yes. write final footer
            groupBuilder.endRecord();
            Logger::debug(String("Stored record group ") + lastGroup + " containing " + groupBuilder.getChildRecordCount() + " records");
        }

        return groupCount;
    }

    uint32_t GameCache::storeCacheMods(MGFDataWriter &writer)
    {
        // FIXME: Same as in storeCache(). Combined cache is crap. Make method better
        std::vector<RecordReflector*> combinedCache;

        // build combined cache with all elements that need a MODIFY record
        for(uint32_t i = 0; i < mBaseCache.size(); ++i)
        {
            // we only need to store a MODIFY for a reflected that was not created at runtime and is dirty
            if(mBaseCache[i]->getCreatedUID().ordinal != UID::SELF_REF_ORDINAL && mBaseCache[i]->isDirty())
            {
                combinedCache.push_back(mBaseCache[i]);
            }
        }

        for(uint32_t i = 0; i < mWorldCache.size(); ++i)
        {
            // we only need to store a MODIFY for a reflected that was not created at runtime and is dirty
            if(mWorldCache[i]->getCreatedUID().ordinal != UID::SELF_REF_ORDINAL && mWorldCache[i]->isDirty())
            {
                combinedCache.push_back(mWorldCache[i]);
            }
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


        // next, iterate over records and store MODIFYs
        RecordBuilder groupBuilder(writer);
        bool writtenStuff = false;

        for(uint32_t i = 0; i < combinedCache.size(); ++i)
        {
            RecordReflector *reflector = combinedCache[i];

            if(!writtenStuff)
            {
                writtenStuff = true;

                groupBuilder.beginGroupRecord(Record::TYPE_MODIFY);

                // set the top group flag
                groupBuilder.setFlags(groupBuilder.getFlags() | RecordHeader::FLAG_TOP_GROUP);
            }

            RecordBuilder childBuilder = groupBuilder.createChildBuilder();
            childBuilder.beginRecord(Record::TYPE_MODIFY, 0, UID::NO_ID);

            // write metadata record
            childBuilder.beginSubrecord(Record::SUBTYPE_MODIFY_METADATA)
                    << reflector->getCreatedUID()
                    << reflector->getRecordType()
                    << static_cast<uint8_t>(0); // dummy modification type
            childBuilder.endSubrecord();

            reflector->storeToModifyRecord(childBuilder);
            childBuilder.endRecord();
        }

        // have we written any MODIFYs?
        if(writtenStuff)
        {
            // yes. write final footer
            groupBuilder.endRecord();

            return 1; // ATM, we never write more than a one single MODIFY group
        }

        return 0;
    }

}
