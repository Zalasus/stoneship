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
#include "MasterGameFile.h"
#include "IWorld.h"
#include "Logger.h"

namespace Stoneship
{

    GameCache::GameCache(MGFManager *mgfManager)
    : mMGFManager(mgfManager)
    , mPolicy(POLICY_KEEP_NEEDED)
    , mLRULimit(3)
    {
    }

    GameCache::~GameCache()
    {
        //FIXME: This kind of management is error-prone and unelegant. Should store an RAII wrapper/smart pointer
        //  in mCache which could manage LRU stuff, too
        for(auto it = mCache.begin(); it != mCache.end(); ++it)
        {
            delete it->second;
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

    UID GameCache::editorNameToUID(const String &name)
    {
        
        for(auto it = mCache.begin(); it != mCache.end(); ++it)
        {
            if(it->second->getEditorName() == name)
            {
                return it->second->getCreatedUID();
            }
        }


        for(uint32_t i = 0; i < mMGFManager->getLoadedMGFCount(); ++i)
        {
            try
            {
                RecordAccessor record = mMGFManager->getLoadedMGFByIndex(i)->getRecordByEditorName(name);

                return record.getUID();

            }catch(StoneshipException &e)
            {
                if(e.getType() != StoneshipException::RECORD_NOT_FOUND)
                {
                    throw;
                }
            }
        }

        STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Could not resolve editor name '" + name + "'");
    }

    /*IEntityBase *GameCache::getBase(const UID &uid, Record::Type type)
    {
        // TODO: linear search complexity. Dunno if this could be more efficient. Maybe if we move found elements to front so often used elements are found more quickly?
        for(uint32_t i = 0; i < mBaseCache.size(); ++i)
        {
            if(mBaseCache[i]->getUID() == uid)
            {
                return mBaseCache[i];
            }
        }


        // base object is not yet cached. aquire it.

        RecordAccessor rec = mMGFManager->getRecordByTypeID(uid, type);

        EntityBaseFactory *factory = EntityBaseFactory::getFactoryForRecordType(rec.getHeader().type);
        if(factory == nullptr)
        {
            STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, String("Record type ") + rec.getHeader().type + " does not match any registered Entity Base types.");
        }

        IEntityBase *base = factory->createEntityBase(uid);

        try
        {
            base->loadFromRecord(rec, this);
            rec.skip();
            base->postLoad(rec, rec, this); // FIXME: second parameter should be the surrounding record. it's not used ATM, so this should be fine, but someday this will produce a hell lot of bugs

        }catch(StoneshipException &e)
        {
            if(e.getType() == StoneshipException::IO_ERROR)
            {
                STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Malformed record or IO error. Could not load entity base.");
            }

            throw;
        }

        // apply modifications. do this only for records that were loaded from MGFs, as SGFs will never modify their own records and MGFs don't see SGF's records
        if(base->getCreatedUID().id != UID::SELF_REF_ORDINAL)
        {
            try
            {
                mMGFManager->applyModifications(base, this);

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
    }*/

    uint32_t GameCache::getCacheSize()
    {
        return mCache.size();
    }

    void GameCache::collectGarbage()
    {
        auto it = mCache.begin();
        while(it != mCache.end())
        {
            IEntityBase *base = dynamic_cast<IEntityBase*>(it->second);
            if(base != nullptr)
            {
                // should we delete this base?
                bool del;
                switch(mPolicy)
                {
                case POLICY_KEEP_NEEDED:
                    // don't delete a base that's still used, is dirty or was created ingame
                    del = ((base->getUserCount() == 0) && 
                           !base->isDirty() && 
                           (base->getCreatedUID().ordinal == UID::SELF_REF_ORDINAL));
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

                    it = mCache.erase(it);
                    continue;
                }
            }
            
            // keep all other cached elements for now
            
            ++it;
        }
    }

    uint32_t GameCache::storeCache(MGFDataWriter &writer, bool cleanAfterStore)
    {
        if(mCache.size() == 0)
        {
            // nothing to store. skip!

            return 0;
        }

        // first, we sort the cache so we can archive proper grouping in the MGF.
        //  since the cache is a map we can't sort that easily, we dump it in a vector first
        std::vector<std::pair<UID, RecordReflector*>> linearCache(mCache.begin(), mCache.end());

        // lambda for comparing record type of two Base objects
        auto compare = [](std::pair<UID, RecordReflector*> a, std::pair<UID, RecordReflector*> b) -> bool { return a.second->getRecordType() < b.second->getRecordType();};

        std::sort(linearCache.begin(), linearCache.end(), compare);


        // next, iterate over records and store them
        Record::Type lastGroup = Record::TYPE_RESERVED;
        uint32_t groupCount = 0;
        RecordBuilder groupBuilder(writer);

        for(auto it = linearCache.begin(); it != linearCache.end(); ++it)
        {
            RecordReflector *reflector = it->second;

            if(reflector->getCreatedUID().ordinal != UID::SELF_REF_ORDINAL)
            {
                // this element was loaded from an MGF. no need to store it
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
                    Logger::debug(String("Stored record group ") + lastGroup + " containing " + groupBuilder.getChildRecordCount() + " records");
                    groupBuilder.beginGroupRecord(reflector->getRecordType(), RecordHeader::FLAG_TOP_GROUP);

                }else
                {
                    // no, this is our first record group. no need to write footer. just start group right ahead
                    groupBuilder.beginGroupRecord(reflector->getRecordType(), RecordHeader::FLAG_TOP_GROUP);
                }

                ++groupCount;
                lastGroup = reflector->getRecordType();
            }


            RecordBuilder childBuilder = groupBuilder.createChildBuilder();
            childBuilder.beginRecord(reflector->getRecordType(), 0, reflector->getCreatedUID().id);
            reflector->storeToRecord(childBuilder);
            bool needsAttachment = childBuilder.getFlags() & RecordHeader::FLAG_ATTACHMENT;
            childBuilder.endRecord();
            if(needsAttachment)
            {
                reflector->storeAttachment(groupBuilder);
            }
            
            /*if(cleanAfterStore)
            {
                // now that we stored the element
                
                reflector->clean();
            }*/
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
        if(mCache.size() == 0)
        {
            // nothing to store. skip!

            return 0;
        }

        //FIXME: Redundant sorting code and data. Unify linearCache in member and avoid unneccesary dumping
        // first, we sort the cache so we can archive proper grouping in the MGF.
        //  since the cache is a map we can't sort that easily, we dump it in a vector first
        std::vector<std::pair<UID, RecordReflector*>> linearCache(mCache.begin(), mCache.end());

        // lambda for comparing record type of two Base objects
        auto compare = [](std::pair<UID, RecordReflector*> a, std::pair<UID, RecordReflector*> b) -> bool { return a.second->getRecordType() < b.second->getRecordType();};

        std::sort(linearCache.begin(), linearCache.end(), compare);


        // next, iterate over records and store MODIFYs
        RecordBuilder groupBuilder(writer);
        bool writtenStuff = false;

        for(auto it = linearCache.begin(); it != linearCache.end(); ++it)
        {
            RecordReflector *reflector = it->second;

            
            if(reflector->getCreatedUID().ordinal == UID::SELF_REF_ORDINAL || reflector->isDirty())
            {
                // if element was created or loaded from savegame or simply is not dirty, no MODIFY is needed
                
                continue;
            }
            
            
            if(!writtenStuff)
            {
                writtenStuff = true;

                groupBuilder.beginGroupRecord(Record::TYPE_MODIFY, RecordHeader::FLAG_TOP_GROUP);
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

    IEntityBase *GameCache::_aquireBase(const UID &uid)
    {
        RecordAccessor rec = mMGFManager->getRecordByID(uid);

        EntityBaseFactory *factory = EntityBaseFactory::getFactoryForRecordType(rec.getHeader().type);
        if(factory == nullptr)
        {
            STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, String("Record type ") + rec.getHeader().type + " does not match any registered Entity Base types.");
        }

        IEntityBase *base = factory->createEntityBase(uid);

        try
        {
            base->loadFromRecord(rec, this);
            bool needsAttachment = rec.getHeader().flags & RecordHeader::FLAG_ATTACHMENT;
            rec.skip();
            if(needsAttachment)
            {
                base->loadAttachment(rec, this);
            }

        }catch(StoneshipException &e)
        {
            if(e.getType() == StoneshipException::IO_ERROR)
            {
                STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Malformed record or IO error. Could not load entity base.");
            }

            throw;
        }

        // apply modifications. do this only for records that were loaded from MGFs, as SGFs will never modify their own records and MGFs don't see SGF's records
        if(base->getCreatedUID().id != UID::SELF_REF_ORDINAL)
        {
            try
            {
                mMGFManager->applyModifications(base, this);

            }catch(StoneshipException &e)
            {
                if(e.getType() == StoneshipException::IO_ERROR)
                {
                    STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Malformed modify record or IO error. Could not apply modifications to base.");
                }

                throw;
            }
        }

        return base;
    }

}
