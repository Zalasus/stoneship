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

	void EntityBased_Book::load(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_WORLD).readStruct<WorldObjectBase>(*this);
		record.getReaderForSubrecord(Record::SUBTYPE_ITEM).readStruct<ItemBase>(*this);

		mText = record.getReaderForSubrecord(Record::SUBTYPE_DATA).readIString();
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




	EntityBase_Weapon::EntityBase_Weapon(Record::Type recordType, UID uid)
	: ItemBase(recordType, uid),
	  mWeaponType(SWORD_ONE_HAND),
	  mDamage(0),
	  mDurability(0),
	  mReach(0)
	{
	}

	void EntityBase_Weapon::load(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_WORLD).readStruct<WorldObjectBase>(*this);
		record.getReaderForSubrecord(Record::SUBTYPE_ITEM).readStruct<ItemBase>(*this);

		MGFDataReader ds = record.getReaderForSubrecord(Record::SUBTYPE_DATA);

		mWeaponType = static_cast<WeaponType>(ds.readUByte());
		mDamage = ds.readUInt();
		mDurability = ds.readUInt();
		mReach = ds.readUInt();
	}

	void EntityBase_Weapon::modify(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_WORLD).readStruct<WorldObjectBase>(*this);
		record.getReaderForSubrecord(Record::SUBTYPE_ITEM).readStruct<ItemBase>(*this);

		MGFDataReader ds = record.getReaderForSubrecord(Record::SUBTYPE_DATA);

		mWeaponType = static_cast<WeaponType>(ds.readUByte());
		mDamage = ds.readUInt();
		mDurability = ds.readUInt();
		mReach = ds.readUInt();
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


	EntityBase_Stuff::EntityBase_Stuff(Record::Type recordType, UID uid)
	: ItemBase(recordType, uid)
	{
	}

	void EntityBase_Stuff::load(RecordAccessor record)
	{
		record.getReaderForSubrecord(Record::SUBTYPE_WORLD).readStruct<WorldObjectBase>(*this);
		record.getReaderForSubrecord(Record::SUBTYPE_ITEM).readStruct<ItemBase>(*this);
	}

	bool EntityBase_Stuff::onUse(ItemStack &stack)
	{
		return false;
	}

}

