/*
 * Item.cpp
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#include "ItemBase.h"
#include "MGFDataReader.h"


namespace Stoneship
{

	template <>
	MGFDataReader &MGFDataReader::readStruct<ItemBase>(ItemBase &base)
	{
		base.mFlags = readUByte();
		base.mName = readBString();
		base.mDescription = readSString();
		base.mValue = readUInt();
		base.mSlots = readUByte();
		base.mIconFile = readBString();

		if(base.isUnidentified())
		{
			readStruct<UID>(base.mIdentified);
		}

		return *this;
	}


	ItemBase::ItemBase(Record::Type recordType, UID uid)
	: WorldObjectBase(recordType, uid),
	  mFlags(0),
	  mValue(0),
	  mSlots(0)
	{
	}

	String ItemBase::getDisplayName() const
	{
		return mName;
	}

	String ItemBase::getDescription() const
	{
		return mDescription;
	}

	uint32_t ItemBase::getValue() const
	{
		return mValue;
	}

	uint8_t ItemBase::getSlots() const
	{
		return mSlots;
	}

	String ItemBase::getIconFile() const
	{
		return mIconFile;
	}

	UID ItemBase::getIdentifiedUID() const
	{
		return mIdentified;
	}
}

