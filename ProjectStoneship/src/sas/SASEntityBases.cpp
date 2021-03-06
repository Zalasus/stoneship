/*
 * SASEntityBases.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "sas/SASEntityBases.h"

#include <iostream>

#include "sas/SASEntities.h"
#include "IEntity.h"
#include "RecordAccessor.h"
#include "RecordBuilder.h"
#include "Root.h"
#include "GameCache.h"
#include "Logger.h"
#include "EventPipeline.h"

namespace Stoneship
{


	EntityBase_Static::EntityBase_Static(UID uid)
	: IEntityBaseWorld(uid)
	{
	}

	bool EntityBase_Static::canInteract() const
	{
	    return false;
	}

	bool EntityBase_Static::onInteract(IEntity *entity, IActor *actor, EventPipeline *pipeline)
	{
		return false;
	}



	EntityBase_Book::EntityBase_Book(UID uid)
	: IEntityBaseItem(uid),
	  mText("", Record::SUBTYPE_TEXT, this)
	{
	}

	bool EntityBase_Book::onUse(ItemStack *stack, IActor *actor)
	{
		std::cout << mText.get() << std::endl;

		return true;
	}

	String EntityBase_Book::getText() const
	{
		return mText.get();
	}

	void EntityBase_Book::setText(const String &s)
	{
	    mText.set(s);
	}




	EntityBase_Weapon::EntityBase_Weapon(UID uid)
	: IEntityBaseItem(uid),
	  mWeaponType(0, Record::SUBTYPE_DATA, this),
	  mDamage(0, Record::SUBTYPE_DATA, this),
	  mDurability(0, Record::SUBTYPE_DATA, this),
	  mReach(0, Record::SUBTYPE_DATA, this)
	{
	}

	EntityBase_Weapon::WeaponType EntityBase_Weapon::getWeaponType() const
	{
		return mWeaponType.get();
	}

	uint32_t EntityBase_Weapon::getDamage() const
	{
		return mDamage.get();
	}

	uint32_t EntityBase_Weapon::getDurability() const
	{
		return mDurability.get();
	}

	uint32_t EntityBase_Weapon::getReach() const
	{
		return mReach.get();
	}

	bool EntityBase_Weapon::onUse(ItemStack *stack, IActor *actor)
	{
		return false;
	}



	EntityBase_Stuff::EntityBase_Stuff(UID uid)
	: IEntityBaseItem(uid)
	{
	}

	bool EntityBase_Stuff::onUse(ItemStack *stack, IActor *actor)
	{
		return false;
	}




	EntityBase_Container::EntityBase_Container(UID uid)
	: IEntityBaseWorld(uid),
	  //mPredefindedInventory(0)
	  mSlotCount(0, Record::SUBTYPE_CONTAINER, this)
	{
	}

    void EntityBase_Container::loadFromRecord(RecordAccessor &record, GameCache *gameCache)
    {
        IEntityBaseWorld::loadFromRecord(record, gameCache);

        /*uint32_t slotCount;
        uint32_t storedItemCount;
        record.getReaderForSubrecord(Record::SUBTYPE_CONTAINER)
                >> slotCount
                >> storedItemCount
                >> MGFDataReader::endr;

        mPredefindedInventory.setSlotCount(slotCount);

        // load item records
        uint32_t itemCount = record.getSubrecordCountForType(Record::SUBTYPE_CONTAINED_ITEM);

        if(storedItemCount != itemCount)
        {
            Logger::warn(getUID().toString() + ": Item count recorded different than count actually stored");
        }

        while((itemCount--) > 0)
        {
            MGFDataReader reader = record.getReaderForSubrecord(Record::SUBTYPE_CONTAINED_ITEM);
            _loadSingleContainedItem(reader);
        }*/
    }

    void EntityBase_Container::loadFromModifyRecord(RecordAccessor &record, GameCache *gameCache)
    {
        IEntityBaseWorld::loadFromModifyRecord(record, gameCache);

        // adding item records
        /*uint32_t itemCount = record.getSubrecordCountForType(Record::SUBTYPE_CONTAINED_ITEM);

        while((itemCount--) > 0)
        {
            MGFDataReader reader = record.getReaderForSubrecord(Record::SUBTYPE_CONTAINED_ITEM);
            _loadSingleContainedItem(reader);
        }*/
    }

    void EntityBase_Container::storeToRecord(RecordBuilder &record)
    {
        IEntityBaseWorld::storeToRecord(record);

        /*record.beginSubrecord(Record::SUBTYPE_CONTAINER)
                << mPredefindedInventory.getSlotCount()
                << mPredefindedInventory.getItems().size();
        record.endSubrecord();


        const Inventory::ItemVector &items = mPredefindedInventory.getItems();

        for(uint32_t i = 0; i < items.size(); ++i)
        {
            record.beginSubrecord(Record::SUBTYPE_CONTAINED_ITEM)
                    << items[i].getItemBase()->getUID()
                    << items[i].getCount();
            record.endSubrecord();
        }*/
    }

    void EntityBase_Container::storeToModifyRecord(RecordBuilder &record)
    {
        IEntityBaseWorld::storeToModifyRecord(record);
    }

    bool EntityBase_Container::canInteract() const
    {
        return true;
    }

    bool EntityBase_Container::onInteract(IEntity *entity, IActor *actor, EventPipeline *pipeline)
    {
        return true;
    }

    /*Inventory &EntityBase_Container::getPredefinedInventory()
    {
        return mPredefindedInventory;
    }*/

    uint32_t EntityBase_Container::getSlotCount() const
    {
        return mSlotCount.get();
    }

    void EntityBase_Container::setSlotCount(uint32_t i)
    {
        mSlotCount.set(i);
    }

    IEntity *EntityBase_Container::createEntity(UID entityUID)
    {
        return new EntityContainer(entityUID, this);
    }

    /*void EntityBase_Container::_loadSingleContainedItem(MGFDataReader &reader)
    {
        UID itemUID;
        uint32_t itemCount;

        reader >> itemUID >> itemCount;
        reader.skipToEnd();

        // store offset. base lookup may move stream pointer TODO: Ugly. Please find better way to avoid skipping through the file like this (indexing item records first?)
        std::streampos pos = reader.tell();

        IEntityBase *base = Root::getSingleton()->getGameCache().getBase(itemUID);
        if(base == nullptr)
        {
            STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Could not find base for contained item " + itemUID.toString());
        }

        mPredefindedInventory.addItem(base, itemCount);

        //restore old position in case it changed
        reader.seek(pos);
    }*/

    REGISTER_ENTITY_BEGIN
        REGISTER_ENTITY_BASE(0x800, EntityBase_Static,    Static,    false)
        REGISTER_ENTITY_BASE(0x810, EntityBase_Weapon,    Weapon,    false)
        REGISTER_ENTITY_BASE(0x820, EntityBase_Book,      Book,      false)
        REGISTER_ENTITY_BASE(0x821, EntityBase_Stuff,     Stuff,     false)
        REGISTER_ENTITY_BASE(0x82A, EntityBase_Container, Container, false)
    REGISTER_ENTITY_END

}

