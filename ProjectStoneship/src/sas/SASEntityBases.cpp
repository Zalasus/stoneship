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

	EntityBase_Book::EntityBase_Book(Record::Type recordType, UID uid)
	: ItemBase(recordType, uid)
	{
	}

	void EntityBase_Book::loadFromRecord(RecordAccessor record)
	{
		WorldObjectBase::loadFromRecord(record);
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

	String EntityBase_Book::getText() const
	{
		return mText;
	}

	bool EntityBase_Book::onUse(ItemStack &stack)
	{
		std::cout << mText << std::endl;

		return true;
	}

	String EntityBase_Book::getBaseName()
	{
		return "Book";
	}




	EntityBase_Weapon::EntityBase_Weapon(Record::Type recordType, UID uid)
	: ItemBase(recordType, uid),
	  mWeaponType(SWORD_ONE_HAND),
	  mDamage(0),
	  mDurability(0),
	  mReach(0)
	{
	}

	void EntityBase_Weapon::loadFromRecord(RecordAccessor record)
	{
		WorldObjectBase::loadFromRecord(record);
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

	bool EntityBase_Weapon::onUse(ItemStack &stack)
	{
		return false;
	}

	String EntityBase_Weapon::getBaseName()
	{
		switch(mWeaponType)
		{
		case SWORD_ONE_HAND:
		case SWORD_TWO_HAND:
			return "Sword";

		case BLUNT:
			return "... blunt weapon";

		case BOW:
			return "Bow";
		}
		return "Weapon";
	}


	EntityBase_Stuff::EntityBase_Stuff(Record::Type recordType, UID uid)
	: ItemBase(recordType, uid)
	{
	}

	void EntityBase_Stuff::loadFromRecord(RecordAccessor record)
	{
		WorldObjectBase::loadFromRecord(record);
		ItemBase::loadFromRecord(record);
	}

	void EntityBase_Stuff::modifyFromRecord(RecordAccessor record)
	{
		loadFromRecord(record);
	}

	bool EntityBase_Stuff::onUse(ItemStack &stack)
	{
		return false;
	}

	String EntityBase_Stuff::getBaseName()
	{
		return "Thing";
	}

}

