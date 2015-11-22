/*
 * WorldObjectBase.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "WorldEntityBase.h"
#include "MGFDataReader.h"


namespace Stoneship
{

	WorldEntityBase::WorldEntityBase(UID uid)
	: EntityBase(uid),
	  mModelScale(0)
	{
	}

	WorldEntityBase::~WorldEntityBase()
	{

	}

	String WorldEntityBase::getModelName() const
	{
		return mModelName;
	}

	float WorldEntityBase::getModelScale() const
	{
		return mModelScale;
	}

	void WorldEntityBase::loadFromRecord(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_WORLD_MODEL)
				.readBString(mModelName)
				.readFloat(mModelScale);
	}

}
