/*
 * Entity.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "Entity.h"

#include "WorldEntityBase.h"
#include "WorldManager.h"

namespace Stoneship
{


	WorldEntity::WorldEntity(UID uid, WorldEntityBase *base, WorldManager *worldManager)
	: mUID(uid),
	  mBase(base),
	  mWorldManager(worldManager),
	  mHidden(false)
	{
		if(mBase != nullptr)
		{
			mBase->mUserCount++;
		}
	}

	WorldEntity::~WorldEntity()
	{
		if((mBase != nullptr) && (mBase->mUserCount > 0))
		{
			mBase->mUserCount--;
		}
	}

	UID WorldEntity::getUID() const
	{
		return mUID;
	}

	WorldEntityBase *WorldEntity::getBase() const
	{
		return mBase;
	}

	void WorldEntity::remove()
	{
		mWorldManager->removeEntity(mUID);
	}

	bool WorldEntity::isHidden() const
	{
		return mHidden;
	}

	void WorldEntity::setHidden(bool hidden)
	{
		mHidden = hidden;
	}
}



