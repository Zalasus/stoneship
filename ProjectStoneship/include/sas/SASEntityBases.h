/*
 * SASEntityBases.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_SAS_SASENTITYBASES_H_
#define INCLUDE_SAS_SASENTITYBASES_H_

#include "ItemBase.h"
#include "Entity.h"
#include "WorldObjectBase.h"

namespace Stoneship
{

	class EntityBase_Book : public ItemBase
	{
	public:

		EntityBase_Book(Record::Type recordType, UID uid);


		void loadFromRecord(RecordAccessor record);
		void modifyFromRecord(RecordAccessor record);
		bool onUse(ItemStack &stack);
		String getBaseName();

		String getText() const;


	private:

		String mText;

	};

	class EntityBase_Weapon : public ItemBase
	{
	public:

		enum WeaponType
		{
			SWORD_ONE_HAND = 0,
			SWORD_TWO_HAND = 1,
			BOW = 2,
			BLUNT = 3
		};

		EntityBase_Weapon(Record::Type recordType, UID uid);


		void loadFromRecord(RecordAccessor record);
		void modifyFromRecord(RecordAccessor record);
		bool onUse(ItemStack &stack);
		String getBaseName();


		WeaponType getWeaponType() const;
		uint32_t getDamage() const;
		uint32_t getDurability() const;
		uint32_t getReach() const;


	private:

		WeaponType mWeaponType;
		uint32_t mDamage;
		uint32_t mDurability;
		uint32_t mReach;

	};

	class EntityBase_Stuff : public ItemBase
	{
	public:

		EntityBase_Stuff(Record::Type recordType, UID uid);

		void loadFromRecord(RecordAccessor record);
		void modifyFromRecord(RecordAccessor record);
		bool onUse(ItemStack &stack);
		String getBaseName();

	};


}

#endif /* INCLUDE_SAS_SASENTITYBASES_H_ */
