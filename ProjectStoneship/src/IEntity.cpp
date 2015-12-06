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



	EntityWorld::EntityWorld(UID uid, IEntityBase *base)
	: IEntity(uid, base),
	  mWorld(nullptr),
	  mHidden(false)
	{
	}

	EntityWorld::~EntityWorld()
	{
	}

	void EntityWorld::spawn(IWorld *w)
	{
	    mWorld = w;
	}

	void EntityWorld::remove()
	{
	    if(mWorld != nullptr)
	    {
	        mWorld->removeEntity(this);
	    }
	}

	bool EntityWorld::isHidden() const
	{
		return mHidden;
	}

	void EntityWorld::setHidden(bool hidden)
	{
		mHidden = hidden;
	}

	void EntityWorld::loadFromRecord(RecordAccessor rec)
	{

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

	void EntityItem::loadFromRecord(RecordAccessor rec)
	{
	    rec.getReaderForSubrecord(Record::SUBTYPE_ENTITY_ITEM)
	            .readUInt(mCount);
	}
}



