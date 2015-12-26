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
#include "EntityManager.h"

namespace Stoneship
{

    WorldDungeon::WorldDungeon(UID uid)
    : IWorld(uid)
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
        return mDungeonName;
    }

    const std::vector<IEntity*> &WorldDungeon::getLoadedEntities()
    {
        return mEntities;
    }

    uint32_t WorldDungeon::getLoadedEntityCount() const
    {
        return mEntities.size();
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

    void WorldDungeon::loadFromRecord(RecordAccessor rec)
    {
        rec.getReaderForSubrecord(Record::SUBTYPE_DATA)
                .readBString(mDungeonName);


        RecordHeader header;
        rec.getReaderForSubrecord(Record::SUBTYPE_SUBGROUP)
                .readStruct(header);
        RecordAccessor subgroup(header, rec.getGameFile(), header.dataSize);

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
                        .readStruct<UID>(baseUID)
                        .readIntegral<Record::Type>(baseType);

                IEntityBase *base = Root::getSingleton()->getEntityManager()->getBase(baseUID, baseType);
                if(base == nullptr)
                {
                    STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Base " + baseUID.toString() + " for Entity " + entityUID.toString() + " not found");
                }

                //The base lookup might have accessed the MGF and thus changed the pointers location. Reset it. FIXME: we can't do this everytime. it's hard to debug where the rollback was omitted
                child.rollback();

                IEntity *entity = base->createEntity(entityUID);
                mEntities.push_back(entity);
                entity->loadFromRecord(child);
                entity->spawn(this);

                if(i < subgroup.getHeader().recordCount-1) //reached end of list yet?
                {
                    //no, fetch next entity record
                    child = child.getNextRecord();
                }
            }// for each entity record
        } //if entity list not empty

    }

}


