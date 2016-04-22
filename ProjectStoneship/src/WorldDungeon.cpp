/*
 * WorldDungeon.h
 *
 *  Created on: Dec 3, 2015
 *      Author: zalasus
 */

#include "WorldDungeon.h"

#include "IEntity.h"
#include "IEntityBase.h"
#include "Root.h"
#include "MasterGameFile.h"
#include "GameCache.h"
#include "RecordBuilder.h"

namespace Stoneship
{

    WorldDungeon::WorldDungeon(UID uid)
    : IWorld(uid),
      mDungeonName("Empty cell", Record::SUBTYPE_DISPLAY_NAME, this)
    {
    }

    WorldDungeon::~WorldDungeon()
    {
        for(uint32_t i = 0; i < mEntities.size(); ++i)
        {
            delete mEntities[i];
        }
    }

    String WorldDungeon::getWorldName() const
    {
        return mDungeonName.get();
    }

    void WorldDungeon::setWorldName(const String &s)
    {
        mDungeonName.set(s);
    }

    const std::vector<IEntity*> &WorldDungeon::getLoadedEntities()
    {
        return mEntities;
    }

    uint32_t WorldDungeon::getLoadedEntityCount() const
    {
        return mEntities.size();
    }

    void WorldDungeon::addEntity(IEntity *entity)
    {
        if(entity->getWorld() == this)
        {
            return;
        }

        mEntities.push_back(entity);
    }

    void WorldDungeon::removeEntity(IEntity *entity)
    {
        auto it = mEntities.begin();
        while(it != mEntities.end())
        {
            if(*it == entity)
            {
                (*it)->despawn();

                delete *it;
                it = mEntities.erase(it);

            }else
            {
                ++it;
            }
        }
    }

    void WorldDungeon::removeEntity(UID uid)
    {
        auto it = mEntities.begin();
        while(it != mEntities.end())
        {
            if((*it)->getUID() == uid)
            {
                (*it)->despawn();

                delete *it;
                it = mEntities.erase(it);

            }else
            {
                ++it;
            }
        }
    }

    void WorldDungeon::loadFromRecord(RecordAccessor &rec)
    {
        IWorld::loadFromRecord(rec);
    }

    void WorldDungeon::postLoad(RecordAccessor &last, RecordAccessor &surrounding)
    {
        RecordAccessor subgroup = last.getNextRecord();

        if(subgroup.getHeader().recordCount > 0)
        {
            //entity list is not empty

            RecordAccessor child = subgroup.getFirstChildRecord();
            for(uint32_t i = 0; i < subgroup.getHeader().recordCount; ++i)
            {
                UID entityUID = UID(child.getGameFile()->getOrdinal(), child.getHeader().id);

                UID baseUID;
                Record::Type baseType;
                child.getReaderForSubrecord(Record::SUBTYPE_ENTITY)
                        >> baseUID
                        >> baseType
                        >> MGFDataReader::endr;

                IEntityBase *base = Root::getSingleton()->getGameCache().getBase(baseUID, baseType);
                if(base == nullptr)
                {
                    STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Base " + baseUID.toString() + " for Entity " + entityUID.toString() + " not found");
                }

                IEntity *entity = base->createEntity(entityUID);
                mEntities.push_back(entity);
                entity->loadFromRecord(child);
                entity->spawn(this);

                // reached end of list yet?
                if(i < subgroup.getHeader().recordCount-1)
                {
                    // no, fetch next entity record
                    child = child.getNextRecord();
                }
            }// for each entity record
        } // if entity list not emptyfinished
    }

    void WorldDungeon::storeToRecord(RecordBuilder &record)
    {
        IWorld::storeToRecord(record);

        if(mEntities.size() > 0)
        {
            record.setFlags(record.getFlags() | RecordHeader::FLAG_ATTACHMENT);
        }
    }

    void WorldDungeon::postStore(RecordBuilder &last, RecordBuilder &surrounding)
    {
        IWorld::postStore(last, surrounding);

        RecordBuilder entityGroup = surrounding.createChildBuilder();
        entityGroup.beginGroupRecord(Record::TYPE_ENTITY, 0);

            for(uint32_t i = 0; i < mEntities.size(); ++i)
            {
                IEntity *entity = mEntities[i];

                // skip this entity if it was not created by the same MGF as the dungeon
                // FIXME: What should we do with it, then? New entities are discarded this way
                if(entity->getCreatedUID().ordinal != getCreatedUID().ordinal)
                {
                    continue;
                }

                RecordBuilder entityBuilder = entityGroup.createChildBuilder();
                entityBuilder.beginRecord(entity->getRecordType(), 0, entity->getUID().id);
                entity->storeToRecord(entityBuilder);
                entityBuilder.endRecord();
                entity->postStore(entityBuilder, entityGroup); // FIXME: every storeToRecord needs a postStore, but the latter is easily omitted. check if we can combine the two so only one call is needed
            }

        entityGroup.endRecord();
    }
}


