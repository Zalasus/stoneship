/*
 * SASEntityBases.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "sas/SASEntityBases.h"

#include <iostream>

#include "IEntity.h"

namespace Stoneship
{


	EntityBase_Static::EntityBase_Static(UID uid)
	: IEntityBaseWorld(uid)
	{
	}

	void EntityBase_Static::loadFromRecord(RecordAccessor record)
	{
		IEntityBaseWorld::loadFromRecord(record);
	}

	void EntityBase_Static::modifyFromRecord(RecordAccessor record)
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
	: IEntityBaseItem(uid)
	{
	}

	void EntityBase_Book::loadFromRecord(RecordAccessor record)
	{
		IEntityBaseWorld::loadFromRecord(record);
		IEntityBaseItem::loadFromRecord(record);

		record.getReaderForSubrecord(Record::SUBTYPE_DATA)
				.readIString(mText);
	}

	void EntityBase_Book::modifyFromRecord(RecordAccessor record)
	{
		//TODO: doing it this way is ugly and possibly dangerous. find a better method

		try
		{
			record.getReaderForSubrecord(Record::SUBTYPE_DATA)
					.readIString(mText);

		}catch(StoneshipException &e)
		{
			if(e.getType() != StoneshipException::SUBRECORD_NOT_FOUND)
			{
				throw;
			}
		}
	}

	bool EntityBase_Book::onUse(ItemStack *stack, IActor *actor)
	{
		std::cout << mText << std::endl;

		return true;
	}

	String EntityBase_Book::getText() const
	{
		return mText;
	}



	EntityBase_Weapon::EntityBase_Weapon(UID uid)
	: IEntityBaseItem(uid),
	  mWeaponType(SWORD_ONE_HAND),
	  mDamage(0),
	  mDurability(0),
	  mReach(0)
	{
	}

	void EntityBase_Weapon::loadFromRecord(RecordAccessor record)
	{
		IEntityBaseWorld::loadFromRecord(record);
		IEntityBaseItem::loadFromRecord(record);

		uint8_t weaponType;
		record.getReaderForSubrecord(Record::SUBTYPE_DATA)
			.readUByte(weaponType)
			.readUInt(mDamage)
			.readUInt(mDurability)
			.readUInt(mReach);

		mWeaponType = static_cast<WeaponType>(weaponType);
	}

	void EntityBase_Weapon::modifyFromRecord(RecordAccessor record)
	{
		loadFromRecord(record);
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

	void EntityBase_Stuff::loadFromRecord(RecordAccessor record)
	{
		IEntityBaseWorld::loadFromRecord(record);
		IEntityBaseItem::loadFromRecord(record);
	}

	void EntityBase_Stuff::modifyFromRecord(RecordAccessor record)
	{
		loadFromRecord(record);
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

    void EntityBase_Container::loadFromRecord(RecordAccessor record)
    {
        uint32_t slotCount;
        uint32_t containedItemCount;

        record.getReaderForSubrecord(Record::SUBTYPE_DATA)
                .readUInt(slotCount)
                .readUInt(containedItemCount);

        mPredefindedInventory.setSlotCount(slotCount);

        /*for(uint32_t i = 0; i < containedItemCount; ++i)
        {
            record.getReaderForSubrecord()
        }*/
    }

    void EntityBase_Container::modifyFromRecord(RecordAccessor record)
    {

    }

    bool EntityBase_Container::canInteract() const
    {
        return true;
    }

    bool EntityBase_Container::onInteract(IEntity *entity, IActor *actor)
    {
        return true;
    }

    const Inventory &EntityBase_Container::getInventory() const
    {
        return mPredefindedInventory;
    }

    IEntity *EntityBase_Container::createEntity(UID entityUID)
    {
        return new EntityContainer(entityUID, this);
    }


    REGISTER_ENTITY_BEGIN
        REGISTER_ENTITY_BASE(0x800, EntityBase_Static,    Static)
        REGISTER_ENTITY_BASE(0x810, EntityBase_Weapon,    Weapon)
        REGISTER_ENTITY_BASE(0x820, EntityBase_Book,      Book)
        REGISTER_ENTITY_BASE(0x821, EntityBase_Stuff,     Stuff)
        REGISTER_ENTITY_BASE(0x82A, EntityBase_Container, Container)
    REGISTER_ENTITY_END

}

