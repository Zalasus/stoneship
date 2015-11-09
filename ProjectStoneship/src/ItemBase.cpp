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

	ItemBase::ItemBase(Record::Type recordType, UID uid)
	: WorldObjectBase(recordType, uid),
	  mFlags(0),
	  mValue(0),
	  mSlots(0)
	{
	}

	void ItemBase::loadFromRecord(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_DISPLAY_NAME)
				.readBString(mName);

		record.getReaderForSubrecord(Record::SUBTYPE_DESCRIPTION)
				.readSString(mDescription);

		record.getReaderForSubrecord(Record::SUBTYPE_TRADING)
				.readUInt(mValue);

		record.getReaderForSubrecord(Record::SUBTYPE_INVENTORY)
				.readUByte(mSlots)
				.readUByte(mFlags);

		record.getReaderForSubrecord(Record::SUBTYPE_ICON)
				.readBString(mIconFile);

		if(isUnidentified())
		{
			record.getReaderForSubrecord(Record::SUBTYPE_IDENTIFICATION)
					.readStruct(mIdentified);
		}
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

