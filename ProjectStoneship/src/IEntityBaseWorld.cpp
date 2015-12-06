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
	  mModelScale(0)
	{
	}

	IEntityBaseWorld::~IEntityBaseWorld()
	{

	}

	String IEntityBaseWorld::getModelName() const
	{
		return mModelName;
	}

	float IEntityBaseWorld::getModelScale() const
	{
		return mModelScale;
	}

	IEntity *IEntityBaseWorld::createEntity(UID uid)
	{
	    return new EntityWorld(uid, this);
	}

	void IEntityBaseWorld::loadFromRecord(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_WORLD_MODEL)
				.readBString(mModelName)
				.readFloat(mModelScale);
	}

}
