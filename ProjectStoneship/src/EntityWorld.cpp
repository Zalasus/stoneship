/*
 * WorldObjectBase.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "EntityWorld.h"
#include "MGFDataReader.h"


namespace Stoneship
{

	EntityWorld::EntityWorld(UID uid, Entity *parent, World *world)
	: Entity(uid, parent),
	  mWorld(world),
	  mModelScale(0)
	{
	}

	EntityWorld::~EntityWorld()
	{

	}

	String EntityWorld::getModelName() const
	{
		return mModelName;
	}

	float EntityWorld::getModelScale() const
	{
		return mModelScale;
	}

	void EntityWorld::loadFromRecord(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_WORLD_MODEL)
				.readBString(mModelName)
				.readFloat(mModelScale);
	}

}
