/*
 * Entity.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "IEntity.h"
#include "IEntityBase.h"
#include "IEntityBaseWorld.h"
#include "IWorld.h"
#include "WorldManager.h"
#include "Logger.h"

#include "sas/SASEntityBases.h" //TODO: try not to merge engine and game specific content (see comments below)

namespace Stoneship
{

	IEntity::IEntity(UID uid, IEntityBase *base)
	: mUID(uid),
	  mBase(base)
	{
		if(mBase != nullptr)
		{
			mBase->mUserCount++;
		}
	}

	IEntity::~IEntity()
	{
		if((mBase != nullptr) && (mBase->mUserCount > 0))
		{
			mBase->mUserCount--;
		}
	}

	UID IEntity::getUID() const
	{
		return mUID;
	}

	IEntityBase *IEntity::getBase() const
	{
		return mBase;
	}

	UID::Ordinal IEntity::getCreatedBy()
	{
	    return mUID.ordinal;
	}



	EntityWorld::EntityWorld(UID uid, IEntityBase *base)
	: IEntity(uid, base),
	  mWorld(nullptr)
	{
	}

	EntityWorld::~EntityWorld()
	{
	}

	void EntityWorld::spawn(IWorld *w)
	{
	    mWorld = w;

	    Logger::info("Spawned entity " + getUID().toString() + " in world " + w->getUID().toString());
	}

	void EntityWorld::despawn()
	{
	    Logger::info("Despawned entity " + getUID().toString() + " in world " + mWorld->getUID().toString());
	}

	void EntityWorld::remove()
	{
	    if(mWorld != nullptr)
	    {
	        mWorld->removeEntity(this);
	    }
	}

	void EntityWorld::loadFromRecord(RecordAccessor &rec)
	{

	}

	void EntityWorld::storeToRecord(RecordBuilder &rec)
	{

	}



	EntityContainer::EntityContainer(UID uidOfEntity, IEntityBase *base) //TODO: make second parameter of every IEntity child only accept correct Base types
	: EntityWorld(uidOfEntity, base),
	  mInventory(0)
	{
	    //TODO: no checking. see above comment

	    EntityBase_Container *cont = static_cast<EntityBase_Container*>(base);
	    mInventory.setSlotCount(cont->getInventory().getSlotCount());
	    mInventory.copyItems(cont->getInventory());
	}

	EntityContainer::~EntityContainer()
	{
	}

	void EntityContainer::loadFromRecord(RecordAccessor &rec)
	{
	    //TODO: load item records here
	}

	void EntityContainer::storeToRecord(RecordBuilder &rec)
    {

    }

	Inventory &EntityContainer::getInventory()
	{
	    return mInventory;
	}


	EntityItem::EntityItem(UID uid, IEntityBase *base)
    : EntityWorld(uid, base),
      mCount(0)
    {
    }

	EntityItem::~EntityItem()
    {
    }

	void EntityItem::setCount(uint32_t count)
	{
	    mCount = count;
	}

	uint32_t EntityItem::getCount() const
	{
	    return mCount;
	}

	void EntityItem::loadFromRecord(RecordAccessor &rec)
	{
	    rec.getReaderForSubrecord(Record::SUBTYPE_ENTITY_ITEM)
	            .readUInt(mCount);
	}

	void EntityItem::storeToRecord(RecordBuilder &rec)
    {

    }
}



