/*
 * Item.cpp
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#include <IEntity.h>
#include <IEntityBaseItem.h>
#include "IActor.h"
#include "MGFDataReader.h"
#include "Inventory.h"
#include "Root.h"
#include "EventPipeline.h"

namespace Stoneship
{

	IEntityBaseItem::IEntityBaseItem(UID uid)
	: IEntityBaseWorld(uid),
	  mFlags(0),
	  mValue(0),
	  mSlots(0),
	  mMaxStackSize(0)
	{
	}

	IEntityBaseItem::~IEntityBaseItem()
	{
	}

	void IEntityBaseItem::loadFromRecord(RecordAccessor record)
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

	String IEntityBaseItem::getDisplayName() const
	{
		return mName;
	}

	String IEntityBaseItem::getDescription() const
	{
		return mDescription;
	}

	uint32_t IEntityBaseItem::getValue() const
	{
		return mValue;
	}

	uint8_t IEntityBaseItem::getSlots() const
	{
		return mSlots;
	}

	uint32_t IEntityBaseItem::getMaxStackSize() const
	{
		return mMaxStackSize;
	}

	bool IEntityBaseItem::isStackable() const
	{
		return mMaxStackSize != 1;
	}

	String IEntityBaseItem::getIconFile() const
	{
		return mIconFile;
	}

	UID IEntityBaseItem::getIdentifiedUID() const
	{
		return mIdentified;
	}

	IEntity *IEntityBaseItem::createEntity(UID uid)
	{
	    return new EntityItem(uid, this);
	}

	bool IEntityBaseItem::canInteract() const
	{
	    return true;
	}

	bool IEntityBaseItem::onInteract(IEntity *entity, IActor *actor)
    {
        return _pickupOnInteract(entity, actor);
    }

	bool IEntityBaseItem::_pickupOnInteract(IEntity *entity, IActor *actor)
	{
	    if(entity->getEntityType() & IEntity::ENTITYTYPE_ITEM)
	    {
	        if(Root::getSingleton()->getEventPipeline()->dispatch(Event(Event::TYPE_PICKUP, entity, actor)))
	        {
                EntityItem *itemEntity = static_cast<EntityItem*>(entity);

                uint32_t countRemaining = itemEntity->getCount() - actor->getInventory().addItem(itemEntity->getBase(), itemEntity->getCount());

                if(countRemaining == 0)
                {
                    itemEntity->remove();

                }else
                {
                    itemEntity->setCount(countRemaining);
                }

                return true;
	        }
	    }

		return false;
	}
}

