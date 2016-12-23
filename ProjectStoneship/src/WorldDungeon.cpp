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
    
    void WorldDungeon::attachNodes(osg::Group *group, ResourceManager *resMan)
    {
        mWorldGroup = osg::ref_ptr<osg::Group>(new osg::Group());
        
        group->addChild(mWorldGroup);
        
        // attach entities
        auto it = mEntities.begin();
        while(it != mEntities.end())
        {
            EntityWorld *ent = dynamic_cast<EntityWorld*>(*it);
            
            if(ent != nullptr)
            {
                ent->attachNodes(mWorldGroup.get(), resMan);
            }
        }
    }
    
    void WorldDungeon::detachNodes(osg::Group *group)
    {
        group->removeChild(mWorldGroup);
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

    void WorldDungeon::loadFromRecord(RecordAccessor &rec, GameCache *gameCache)
    {
        IWorld::loadFromRecord(rec, gameCache);
        
        //quePostLoad(rec,gameCache);
    }

    void WorldDungeon::postLoad(RecordAccessor &last, RecordAccessor &surrounding, GameCache *gameCache)
    {
        RecordIterator it = last.toIterator().next();
        
        if(it->getHeader().recordCount > 0)
        {
            //entity list is not empty

            RecordIterator childIt = it->getChildIterator();
            while(childIt != it->getChildEnd())
            {
                RecordAccessor childRecord = *childIt;
                
                UID entityUID = UID(childRecord.getGameFile()->getOrdinal(), childRecord.getHeader().id);

                UID baseUID;
                Record::Type baseType;
                childRecord.getReaderForSubrecord(Record::SUBTYPE_ENTITY)
                        >> baseUID
                        >> baseType
                        >> MGFDataReader::endr;

                IEntityBase *base = gameCache->getCachedElementOfType<IEntityBase>(baseUID, baseType);
                if(base == nullptr) //FIXME: won't be returned
                {
                    STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Base " + baseUID.toString() + " for Entity " + entityUID.toString() + " not found");
                }

                IEntity *entity = base->createEntity(entityUID);
                mEntities.push_back(entity);
                entity->loadFromRecord(childRecord, gameCache);
                entity->spawn(this);
                
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


