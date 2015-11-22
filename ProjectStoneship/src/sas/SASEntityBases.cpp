/*
 * SASEntityBases.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "sas/SASEntityBases.h"

#include <iostream>

namespace Stoneship
{


	EntityBase_Static::EntityBase_Static(UID uid)
	: WorldEntityBase(uid)
	{
	}

	void EntityBase_Static::loadFromRecord(RecordAccessor record)
	{
		WorldEntityBase::loadFromRecord(record);
	}

	void EntityBase_Static::modifyFromRecord(RecordAccessor record)
	{
	}

	bool EntityBase_Static::onInteract(Entity *entity, Actor *actor)
	{
		return false;
	}



	EntityBase_Book::EntityBase_Book(UID uid)
	: ItemBase(uid)
	{
	}

	void EntityBase_Book::loadFromRecord(RecordAccessor record)
	{
		WorldEntityBase::loadFromRecord(record);
		ItemBase::loadFromRecord(record);

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

	bool EntityBase_Book::onUse(ItemStack &stack, Actor &actor)
	{
		std::cout << mText << std::endl;

		return true;
	}

	bool EntityBase_Book::onInteract(Entity *entity, Actor *actor)
	{
		return _pickupOnInteract(entity, actor);
	}

	String EntityBase_Book::getText() const
	{
		return mText;
	}



	EntityBase_Weapon::EntityBase_Weapon(UID uid)
	: ItemBase(uid),
	  mWeaponType(SWORD_ONE_HAND),
	  mDamage(0),
	  mDurability(0),
	  mReach(0)
	{
	}

	void EntityBase_Weapon::loadFromRecord(RecordAccessor record)
	{
		WorldEntityBase::loadFromRecord(record);
		ItemBase::loadFromRecord(record);

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

	bool EntityBase_Weapon::onUse(ItemStack &stack, Actor &actor)
	{
		return false;
	}

	bool EntityBase_Weapon::onInteract(Entity *entity, Actor *actor)
	{
		return _pickupOnInteract(entity, actor);
	}



	EntityBase_Stuff::EntityBase_Stuff(UID uid)
	: ItemBase(uid)
	{
	}

	void EntityBase_Stuff::loadFromRecord(RecordAccessor record)
	{
		WorldEntityBase::loadFromRecord(record);
		ItemBase::loadFromRecord(record);
	}

	void EntityBase_Stuff::modifyFromRecord(RecordAccessor record)
	{
		loadFromRecord(record);
	}

	bool EntityBase_Stuff::onUse(ItemStack &stack, Actor &actor)
	{
		return false;
	}

	bool EntityBase_Stuff::onInteract(Entity *entity, Actor *actor)
	{
		return _pickupOnInteract(entity, actor);
	}

}

