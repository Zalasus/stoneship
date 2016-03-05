/*
 * Entity.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "IEntity.h"
#include "IEntityBase.h"
#include "IEntityBaseWorld.h"
#include "IEntityBaseItem.h"
#include "IWorld.h"
#include "GameCache.h"
#include "Logger.h"
#include "RecordBuilder.h"

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

	UID IEntity::getCreatedUID() const
	{
	    return mUID;
	}

	Record::Type IEntity::getRecordType() const
	{
	    return Record::TYPE_ENTITY;
	}

    void IEntity::storeToRecord(RecordBuilder &record)
    {
        RecordReflector::storeToRecord(record);

        record.beginSubrecord(Record::SUBTYPE_ENTITY, sizeof(UID) + sizeof(Record::Type))
                << mBase->getUID()
                << mBase->getRecordType();
        record.endSubrecord();
    }



	EntityWorld::EntityWorld(UID uid, IEntityBaseWorld *base)
	: IEntity(uid, base),
	  mWorld(nullptr),
	  mScale(1, Record::SUBTYPE_SCALE, this)
	{
	}

	EntityWorld::~EntityWorld()
	{
	}

	IWorld *EntityWorld::getWorld() const
	{
	    return mWorld;
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

	float EntityWorld::getScale() const
	{
	    return mScale.get();
	}

	void EntityWorld::setScale(float f)
	{
	    mScale.set(f);
	}

	void EntityWorld::remove()
	{
	    if(mWorld != nullptr)
	    {
	        mWorld->removeEntity(this);
	    }
	}




	EntityItem::EntityItem(UID uid, IEntityBaseItem *base)
    : EntityWorld(uid, base),
      mCount(0, Record::SUBTYPE_COUNT, this)
    {
    }

	EntityItem::~EntityItem()
    {
    }

	void EntityItem::setCount(uint32_t count)
	{
	    mCount.set(count);
	}

	uint32_t EntityItem::getCount() const
	{
	    return mCount.get();
	}

}



