/*
 * SASEntityBases.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "sas/SASEntityBases.h"

#include <iostream>

#include "IEntity.h"
#include "RecordBuilder.h"
#include "Root.h"
#include "EntityManager.h"

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

	bool EntityBase_Static::onInteract(IEntity *entity, IActor *actor)
	{
		return false;
	}



	EntityBase_Book::EntityBase_Book(UID uid)
	: IEntityBaseItem(uid),
	  mText("", this)
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
	  mWeaponType(TYPE_SWORD_ONE_HAND, this),
	  mDamage(0, this),
	  mDurability(0, this),
	  mReach(0, this)
	{
	}

	EntityBase_Weapon::WeaponType EntityBase_Weapon::getWeaponType() const
	{
		return mWeaponType;
	}

	uint32_t EntityBase_Weapon::getDamage() const
	{
		return mDamage;
	}

	uint32_t EntityBase_Weapon::getDurability() const
	{
		return mDurability;
	}

	uint32_t EntityBase_Weapon::getReach() const
	{
		return mReach;
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
	  mPredefindedInventory(0)
	{
	}

    void EntityBase_Container::loadFromRecord(RecordAccessor &record)
    {
        IEntityBaseWorld::loadFromRecord(record);

        // load item records
        uint32_t itemCount = record.getSubrecordCountForType(Record::SUBTYPE_CONTAINED_ITEM);

        while((itemCount--) > 0)
        {
            MGFDataReader reader = record.getReaderForSubrecord(Record::SUBTYPE_CONTAINED_ITEM);
            _loadSingleContainedItem(reader);
        }
    }

    void EntityBase_Container::loadFromModifyRecord(RecordAccessor &record)
    {
        IEntityBaseWorld::loadFromModifyRecord(record);

        // adding item records TODO: modifying/removing items
        uint32_t itemCount = record.getSubrecordCountForType(Record::SUBTYPE_CONTAINED_ITEM);

        while((itemCount--) > 0)
        {
            MGFDataReader reader = record.getReaderForSubrecord(Record::SUBTYPE_CONTAINED_ITEM);
            _loadSingleContainedItem(reader);
        }
    }

    void EntityBase_Container::storeToRecord(RecordBuilder &record)
    {
        IEntityBaseWorld::storeToRecord(record);

        Inventory::ItemVector &items = mPredefindedInventory.get().getItems();

        for(uint32_t i = 0; i < items.size(); ++i)
        {
            record.beginSubrecord(Record::SUBTYPE_CONTAINED_ITEM)
                    .writeStruct<UID>(items[i].getItemBase()->getUID())
                    .writeUInt(items[i].getCount());
            record.endSubrecord();
        }
    }

    bool EntityBase_Container::canInteract() const
    {
        return true;
    }

    bool EntityBase_Container::onInteract(IEntity *entity, IActor *actor)
    {
        return true;
    }

    Inventory &EntityBase_Container::getPredefinedInventory()
    {
        return mPredefindedInventory;
    }

    IEntity *EntityBase_Container::createEntity(UID entityUID)
    {
        return new EntityContainer(entityUID, this);
    }

    void EntityBase_Container::_loadSingleContainedItem(MGFDataReader &reader)
    {
        UID itemUID;
        uint32_t itemCount;

        reader
                .readStruct<UID>(itemUID)
                .readUInt(itemCount)
                .skipToEnd();

        // store offset. base lookup may move stream pointer TODO: Ugly. Please find better way to avoid skipping through the file like this (indexing item records first?)
        std::streampos pos = reader.tell();

        IEntityBase *base = Root::getSingleton()->getEntityManager()->getBase(itemUID);
        if(base == nullptr)
        {
            STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Could not find base for contained item " + itemUID.toString());
        }

        if(!(base->getBaseType() | IEntityBase::BASETYPE_ITEM))
        {
            STONESHIP_EXCEPT(StoneshipException::INVALID_RECORD_TYPE, "Specified UID for contained item is of non-item type");
        }

        IEntityBaseItem *itemBase = static_cast<IEntityBaseItem*>(base);

        mPredefindedInventory.addItem(ItemStack(itemBase, itemCount));

        //restore old position in case it changed
        reader.seek(pos);
    }

    REGISTER_ENTITY_BEGIN
        REGISTER_ENTITY_BASE(0x800, EntityBase_Static,    Static)
        REGISTER_ENTITY_BASE(0x810, EntityBase_Weapon,    Weapon)
        REGISTER_ENTITY_BASE(0x820, EntityBase_Book,      Book)
        REGISTER_ENTITY_BASE(0x821, EntityBase_Stuff,     Stuff)
        REGISTER_ENTITY_BASE(0x82A, EntityBase_Container, Container)
    REGISTER_ENTITY_END

}

