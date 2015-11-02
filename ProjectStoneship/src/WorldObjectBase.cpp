/*
 * WorldObjectBase.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "WorldObjectBase.h"

#include "MGFDataReader.h"


namespace Stoneship
{

	WorldObjectBase::WorldObjectBase(Record::Type recordType, UID uid)
	: EntityBase(recordType, uid)
	{
	}

	String WorldObjectBase::getModelName() const
	{
		return mModelName;
	}

	void WorldObjectBase::load(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_WORLD_MODEL)
				.readBString(mModelName);
	}

}
