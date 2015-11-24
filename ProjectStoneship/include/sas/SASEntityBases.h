/*
 * SASEntityBases.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_SAS_SASENTITYBASES_H_
#define INCLUDE_SAS_SASENTITYBASES_H_

#include "Types.h"
#include "Record.h"
#include "String.h"
#include "EntityBase.h"
#include "WorldEntityBase.h"
#include "ItemBase.h"

namespace Stoneship
{

    class ItemStack;
    class Entity;

	class EntityBase_Static : public WorldEntityBase
	{
	public:
		EntityBase_Static(UID uid);

		Record::Type getRecordType() const {return 0x800;}
		EntityBase::BaseType getBaseType() const {return BASETYPE_WORLD;};
		String getBaseName() const {return "Static";}

		void loadFromRecord(RecordAccessor record);
		void modifyFromRecord(RecordAccessor record);

		bool onUse(ItemStack *stack, Actor *actor);
		bool onInteract(Entity *entity, Actor *actor);

	};

	class EntityBase_Book : public ItemBase
	{
	public:

		EntityBase_Book(UID uid);

		Record::Type getRecordType() const {return 0x820;}
		EntityBase::BaseType getBaseType() const {return BASETYPE_WORLD | BASETYPE_ITEM;}
		String getBaseName() const {return "Book";}

		void loadFromRecord(RecordAccessor record);
		void modifyFromRecord(RecordAccessor record);

		bool onUse(ItemStack *stack, Actor *actor);
		bool onInteract(Entity *entity, Actor *actor);

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

		EntityBase_Weapon(UID uid);

		Record::Type getRecordType() const {return 0x810;}
		EntityBase::BaseType getBaseType() const {return BASETYPE_ITEM | BASETYPE_WORLD;}
		String getBaseName() const {return "Weapon";}

		void loadFromRecord(RecordAccessor record);
		void modifyFromRecord(RecordAccessor record);

		bool onUse(ItemStack *stack, Actor *actor);
		bool onInteract(Entity *entity, Actor *actor);

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

		EntityBase_Stuff(UID uid);

		Record::Type getRecordType() const {return 0x821;}
		EntityBase::BaseType getBaseType() const {return BASETYPE_ITEM | BASETYPE_WORLD;}
		String getBaseName() const {return "Stuff";}

		void loadFromRecord(RecordAccessor record);
		void modifyFromRecord(RecordAccessor record);

		bool onUse(ItemStack *stack, Actor *actor);
		bool onInteract(Entity *entity, Actor *actor);
	};


}

#endif /* INCLUDE_SAS_SASENTITYBASES_H_ */
