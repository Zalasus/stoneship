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

	template <>
	MGFDataReader &MGFDataReader::readStruct<WorldObjectBase>(WorldObjectBase &base)
	{
		base.mModelName = readBString();

		return *this;
	}


	WorldObjectBase::WorldObjectBase(Record::Type recordType, UID uid)
	: EntityBase(recordType, uid)
	{
	}

	String WorldObjectBase::getModelName() const
	{
		return mModelName;
	}

}
