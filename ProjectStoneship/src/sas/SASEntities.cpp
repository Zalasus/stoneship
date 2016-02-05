/*
 * SASEntities.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: zalasus
 */

#include "sas/SASEntities.h"


namespace Stoneship
{

    EntityContainer::EntityContainer(UID uidOfEntity, EntityBase_Container *base)
    : EntityWorld(uidOfEntity, base),
      mInventory(0)
    {
        mInventory.setSlotCount(base->getPredefinedInventory().getSlotCount());
        mInventory.copyItems(base->getPredefinedInventory());
    }

    EntityContainer::~EntityContainer()
    {
    }

    void EntityContainer::loadFromRecord(RecordAccessor &rec)
    {
        //TODO: load item records here
    }

    void EntityContainer::loadFromModifyRecord(RecordAccessor &rec)
    {

    }

    void EntityContainer::storeToRecord(RecordBuilder &rec)
    {

    }

    void EntityContainer::storeToModifyRecord(RecordBuilder &rec)
    {

    }

    Inventory &EntityContainer::getInventory()
    {
        return mInventory;
    }

}
