/*
 * Item.cpp
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#include <typeinfo>

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
	  mName("", Record::SUBTYPE_DISPLAY_NAME, this),
	  mDescription("", Record::SUBTYPE_DESCRIPTION, this),
	  mValue(0, Record::SUBTYPE_TRADING, this),
	  mInventory(this),
	  mIconFile("empty.png", Record::SUBTYPE_ICON, this),
	  mIdentified(UID::NO_UID, Record::SUBTYPE_IDENTIFICATION, this)
	{
	}

	IEntityBaseItem::~IEntityBaseItem()
	{
	}

	bool IEntityBaseItem::mustStore(SubrecordFieldS *field)
	{
	    if(field == &mIdentified)
	    {
	        return isToBeIdentified();
	    }

	    return IEntityBaseWorld::mustStore(field);
	}

	bool IEntityBaseItem::mustLoad(SubrecordFieldS *field)
	{
	    if(field == &mIdentified)
        {
            return isToBeIdentified();
        }

        return IEntityBaseWorld::mustLoad(field);
	}

	String IEntityBaseItem::getDisplayName() const
	{
		return mName.get();
	}

	void IEntityBaseItem::setDisplayName(const String &s)
	{
	    mName.set(s);
	}

	String IEntityBaseItem::getDescription() const
	{
		return mDescription.get();
	}

	void IEntityBaseItem::setDescription(const String &s)
	{
	    mDescription.set(s);
	}

	uint32_t IEntityBaseItem::getValue() const
	{
		return mValue.get();
	}

	void IEntityBaseItem::setValue(uint32_t i)
	{
	    mValue.set(i);
	}

	uint8_t IEntityBaseItem::getOccupyingSlots() const
	{
		return mInventory.getSlotCount();
	}

	void IEntityBaseItem::setOccupyingSlots(uint8_t i)
	{
	    mInventory.setSlotCount(i);
	}

	uint32_t IEntityBaseItem::getMaxStackSize() const
	{
		return mInventory.getMaxStackSize();
	}

	void IEntityBaseItem::setMaxStackSize(uint8_t i)
	{
	    mInventory.setMaxStackSize(i);
	}

	bool IEntityBaseItem::isStackable() const
	{
		return getMaxStackSize() != 1;
	}

	String IEntityBaseItem::getIconFile() const
	{
		return mIconFile.get();
	}

	void IEntityBaseItem::setIconFile(const String &s)
	{
	    mIconFile.set(s);
	}

	UID IEntityBaseItem::getIdentifiedUID() const
	{
		return mIdentified.get();
	}

	void IEntityBaseItem::setIdentifiedUID(UID id)
	{
	    mIdentified.set(id);
	}

	IEntity *IEntityBaseItem::createEntity(UID uid)
	{
	    return new EntityItem(uid, this);
	}

	bool IEntityBaseItem::canInteract() const
	{
	    return true;
	}

	bool IEntityBaseItem::onInteract(IEntity *entity, IActor *actor, EventPipeline *pipeline)
    {
        return _pickupOnInteract(entity, actor, pipeline);
    }

	bool IEntityBaseItem::_pickupOnInteract(IEntity *entity, IActor *actor, EventPipeline *pipeline)
	{
	    if(typeid(*entity) == typeid(EntityItem))  // TODO: buähhh
	    {
	        if(pipeline->dispatch(Event(Event::TYPE_PICKUP, entity, actor)))
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

