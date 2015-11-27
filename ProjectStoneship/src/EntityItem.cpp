/*
 * Item.cpp
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#include "EntityItem.h"
#include "MGFDataReader.h"
#include "Inventory.h"
#include "Actor.h"
#include "Entity.h"
#include "Root.h"
#include "EventPipeline.h"

namespace Stoneship
{

	EntityItem::EntityItem(UID uid, Entity *parent, World *world)
	: EntityWorld(uid, parent, world),
	  mFlags(0),
	  mValue(0),
	  mSlots(0),
	  mMaxStackSize(0)
	{
	}

	EntityItem::~EntityItem()
	{
	}

	void EntityItem::loadFromRecord(RecordAccessor record)
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

	String EntityItem::getDisplayName() const
	{
		return mName;
	}

	String EntityItem::getDescription() const
	{
		return mDescription;
	}

	uint32_t EntityItem::getValue() const
	{
		return mValue;
	}

	uint8_t EntityItem::getSlots() const
	{
		return mSlots;
	}

	uint32_t EntityItem::getMaxStackSize() const
	{
		return mMaxStackSize;
	}

	bool EntityItem::isStackable() const
	{
		return mMaxStackSize != 1;
	}

	String EntityItem::getIconFile() const
	{
		return mIconFile;
	}

	UID EntityItem::getIdentifiedUID() const
	{
		return mIdentified;
	}

	bool EntityItem::_pickupOnInteract(Actor *actor)
	{
	    if(Root::getSingleton()->getEventPipeline()->dispatch(Event(Event::TYPE_PICKUP, actor)))
        {
            uint32_t countRemaining = getCount() - actor->getInventory().addItem(itemEntity->getBase(), itemEntity->getCount());

            if(countRemaining == 0)
            {
                itemEntity->remove();

            }else
            {
                itemEntity->setCount(countRemaining);
            }

            return true;
        }

		return false;
	}
}

