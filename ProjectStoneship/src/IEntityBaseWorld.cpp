/*
 * WorldObjectBase.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "IEntityBaseWorld.h"
#include "IEntity.h"
#include "MGFDataReader.h"


namespace Stoneship
{

	IEntityBaseWorld::IEntityBaseWorld(UID uid)
	: IEntityBase(uid),
	  mModelName("", Record::SUBTYPE_WORLD_MODEL_NAME, this)
	{
	}

	IEntityBaseWorld::~IEntityBaseWorld()
	{

	}

	String IEntityBaseWorld::getModelName() const
	{
		return mModelName.get();
	}

	IEntity *IEntityBaseWorld::createEntity(UID uid)
	{
	    return new EntityWorld(uid, this);
	}

}
