/*
 * Entity.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "Entity.h"

#include "EntityBase.h"
#include "WorldEntityBase.h"
#include "WorldManager.h"

namespace Stoneship
{

	Entity::Entity(UID uid, EntityBase *base)
	: mUID(uid),
	  mBase(base)
	{
		if(mBase != nullptr)
		{
			mBase->mUserCount++;
		}
	}

	Entity::~Entity()
	{
		if((mBase != nullptr) && (mBase->mUserCount > 0))
		{
			mBase->mUserCount--;
		}
	}

	UID Entity::getUID() const
	{
		return mUID;
	}

	EntityBase *Entity::getBase() const
	{
		return mBase;
	}



	WorldEntity::WorldEntity(UID uid, EntityBase *base, WorldManager *worldManager)
	: Entity(uid, base),
	  mWorldManager(worldManager),
	  mHidden(false)
	{
	}

	WorldEntity::~WorldEntity()
	{
	}

	void WorldEntity::remove()
	{
		mWorldManager->removeEntity(getUID());
	}

	bool WorldEntity::isHidden() const
	{
		return mHidden;
	}

	void WorldEntity::setHidden(bool hidden)
	{
		mHidden = hidden;
	}

	void WorldEntity::loadFromRecord(RecordAccessor rec)
	{

	}



	ItemEntity::ItemEntity(UID uid, EntityBase *base, WorldManager *worldManager)
    : WorldEntity(uid, base, worldManager),
      mCount(0)
    {
    }

	ItemEntity::~ItemEntity()
    {
    }

	void ItemEntity::setCount(uint32_t count)
	{
	    mCount = count;
	}

	uint32_t ItemEntity::getCount() const
	{
	    return mCount;
	}

	void ItemEntity::loadFromRecord(RecordAccessor rec)
	{

	}
}



