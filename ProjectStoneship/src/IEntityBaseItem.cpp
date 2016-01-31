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
	  mName("", this),
	  mDescription("", this),
	  mValue(0, this),
	  mInventory(this),
	  mIconFile("empty.png", this),
	  mIdentified(UID::NO_UID, this)
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
		return mName;
	}

	void IEntityBaseItem::setDisplayName(const String &s)
	{
	    mName = s;
	}

	String IEntityBaseItem::getDescription() const
	{
		return mDescription;
	}

	void IEntityBaseItem::setDescription(const String &s)
	{
	    mDescription = s;
	}

	uint32_t IEntityBaseItem::getValue() const
	{
		return mValue;
	}

	void IEntityBaseItem::setValue(uint32_t i)
	{
	    mValue = i;
	}

	uint8_t IEntityBaseItem::getSlots() const
	{
		return mInventory.getSlotCount();
	}

	void IEntityBaseItem::setSlots(uint8_t i)
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
		return mIconFile;
	}

	void IEntityBaseItem::setIconFile(const String &s)
	{
	    mIconFile = s;
	}

	UID IEntityBaseItem::getIdentifiedUID() const
	{
		return mIdentified;
	}

	void IEntityBaseItem::setIdentifiedUID(UID id)
	{
	    mIdentified = id;
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

