/*
 * Item.cpp
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#include "ItemBase.h"

#include "MGFDataReader.h"
#include "Inventory.h"
#include "Actor.h"
#include "Entity.h"

namespace Stoneship
{

	ItemBase::ItemBase(UID uid)
	: WorldEntityBase(uid),
	  mFlags(0),
	  mValue(0),
	  mSlots(0),
	  mMaxStackSize(0)
	{
	}

	ItemBase::~ItemBase()
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
				.readUByte(mFlags)
				.readUByte(mSlots)
				.readUInt(mMaxStackSize);

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

	uint32_t ItemBase::getMaxStackSize() const
	{
		return mMaxStackSize;
	}

	bool ItemBase::isStackable() const
	{
		return mMaxStackSize != 1;
	}

	String ItemBase::getIconFile() const
	{
		return mIconFile;
	}

	UID ItemBase::getIdentifiedUID() const
	{
		return mIdentified;
	}

	bool ItemBase::_pickupOnInteract(Entity *entity, Actor *actor)
	{
		actor->getInventory().addItem(entity->getBase(), 1); //TODO: Use count field of entity record instead of fixed value

		if(entity->getEntityType() == Entity::ENTITYTYPE_WORLD)
		{
			static_cast<WorldEntity*>(entity)->remove();
		}

		return true;
	}
}

