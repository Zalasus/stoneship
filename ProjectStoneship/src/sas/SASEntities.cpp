/*
 * SASEntities.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: zalasus
 */

#include "sas/SASEntities.h"

#include "Root.h"
#include "RecordBuilder.h"
#include "RecordAccessor.h"
#include "GameCache.h"

namespace Stoneship
{

    EntityContainer::EntityContainer(UID uidOfEntity, EntityBase_Container *base)
    : EntityWorld(uidOfEntity, base),
      mInventory(base->getSlotCount())
    {
        //mInventory.copyItems(base->getPredefinedInventory());
    }

    EntityContainer::~EntityContainer()
    {
    }

    void EntityContainer::loadFromRecord(RecordAccessor &rec, GameCache *gameCache)
    {
        EntityWorld::loadFromRecord(rec, gameCache);

        mInventory.clear(); // remove all items copied from the predefined inventory when loading

        uint32_t containedItems;
        uint32_t slotCount;

        rec.getReaderForSubrecord(Record::SUBTYPE_CONTAINER)
                >> slotCount
                >> containedItems
                >> MGFDataReader::endr;

        mInventory.setSlotCount(slotCount);

        for(uint32_t i = 0; i < containedItems; ++i)
        {
            UID baseUID;
            uint32_t count;

            rec.getReaderForSubrecord(Record::SUBTYPE_CONTAINED_ITEM)
                >> baseUID
                >> count
                >> MGFDataReader::endr;

            std::streampos pos = rec.getReader().tell(); // store position. cache lookup may change stream pointer

            IEntityBase *base = gameCache->getCachedElement<IEntityBase>(baseUID);
            if(base == nullptr) //TODO: will this ever be returned?
            {
                STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Contained item refers to non-existing base " + baseUID.toString());
            }

            rec.getReader().seek(pos);

            mInventory.addItem(base, count);
        }
    }

    void EntityContainer::loadFromModifyRecord(RecordAccessor &rec, GameCache *gameCache)
    {

    }

    void EntityContainer::storeToRecord(RecordBuilder &record)
    {
        EntityWorld::storeToRecord(record);

        record.beginSubrecord(Record::SUBTYPE_CONTAINER)
                << mInventory.getSlotCount()
                << mInventory.getItems().size();
        record.endSubrecord();


        const Inventory::ItemVector &items = mInventory.getItems();

        for(uint32_t i = 0; i < items.size(); ++i)
        {
            record.beginSubrecord(Record::SUBTYPE_CONTAINED_ITEM)
                    << items[i].getItemBase()->getUID()
                    << items[i].getCount();
            record.endSubrecord();
        }
    }

    void EntityContainer::storeToModifyRecord(RecordBuilder &rec)
    {

    }

    Inventory &EntityContainer::getInventory()
    {
        return mInventory;
    }

}
