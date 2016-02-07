/*
 * SASEntityBases.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_SAS_SASENTITYBASES_H_
#define INCLUDE_SAS_SASENTITYBASES_H_

#include "IEntityBase.h"
#include "IEntityBaseItem.h"
#include "IEntityBaseWorld.h"
#include "Types.h"
#include "Record.h"
#include "String.h"
#include "Inventory.h"

namespace Stoneship
{

    class ItemStack;
    class IEntity;
    class RecordAccessor;
    class RecordBuilder;

	class EntityBase_Static : public IEntityBaseWorld
	{
	public:
		EntityBase_Static(UID uid);

		// override RecordReflector
		Record::Type getRecordType() const { return 0x800; }

		// override IEntityBase
		IEntityBase::BaseType getBaseType() const {return BASETYPE_WORLD;};
		String getBaseName() const {return "Static";}

		// override IEntityBaseWorld
		bool canInteract() const;
		bool onInteract(IEntity *entity, IActor *actor);

	};

	class EntityBase_Book : public IEntityBaseItem
	{
	public:

		EntityBase_Book(UID uid);

		// override RecordReflector
		Record::Type getRecordType() const {return 0x820;}

		// override IEntityBase
		IEntityBase::BaseType getBaseType() const {return BASETYPE_WORLD | BASETYPE_ITEM;}
		String getBaseName() const {return "Book";}

		// override IEntityBaseItem
		bool onUse(ItemStack *stack, IActor *actor);

        // interact events from IEntityBaseWorld not overridden. they create pickup event by default

		String getText() const;
		void setText(const String &text);


	private:

		SubrecordField<String> mText;

	};

	class EntityBase_Weapon : public IEntityBaseItem
	{
	public:

		typedef uint8_t WeaponType;
		static const WeaponType TYPE_SWORD_ONE_HAND = 1;
		static const WeaponType TYPE_SWORD_TWO_HAND = 2;
		static const WeaponType TYPE_BOW = 3;
		static const WeaponType TYPE_BLUNT = 4;

		EntityBase_Weapon(UID uid);

		// override RecordReflector
		Record::Type getRecordType() const {return 0x810;}

		// override IEntityBase
		IEntityBase::BaseType getBaseType() const {return BASETYPE_ITEM | BASETYPE_WORLD;}
		String getBaseName() const {return "Weapon";}

        // override IEntityBaseItem
		bool onUse(ItemStack *stack, IActor *actor);

        // interact events from IEntityBaseWorld not overridden. they create pickup event by default

		WeaponType getWeaponType() const;
		uint32_t getDamage() const;
		uint32_t getDurability() const;
		uint32_t getReach() const;


	private:

		SubrecordField<WeaponType> mWeaponType;
		SubrecordField<uint32_t> mDamage;
		SubrecordField<uint32_t> mDurability;
		SubrecordField<uint32_t> mReach;

	};

	class EntityBase_Stuff : public IEntityBaseItem
	{
	public:

		EntityBase_Stuff(UID uid);

		// override RecordReflector
		Record::Type getRecordType() const {return 0x821;}

		// override IEntityBase
		IEntityBase::BaseType getBaseType() const {return BASETYPE_ITEM | BASETYPE_WORLD;}
		String getBaseName() const {return "Stuff";}

		// override IEntityBaseItem
		bool onUse(ItemStack *stack, IActor *actor);

        // interact events from IEntityBaseWorld not overridden. they create pickup event by default
	};

	class EntityBase_Container : public IEntityBaseWorld
	{
	public:
	    EntityBase_Container(UID uid);

	    // override RecordReflector
	    Record::Type getRecordType() const {return 0x82A;}

	    // override IEntityBase
	    IEntityBase::BaseType getBaseType() const {return BASETYPE_WORLD;}
        String getBaseName() const {return "Container";}
        IEntity *createEntity(UID entityUID); // this Base needs a custom Entity that has an Inventory

        // override IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeToModifyRecord(RecordBuilder &record);

        // override IEntityBaseWorld
        bool canInteract() const;
        bool onInteract(IEntity *entity, IActor *actor);

        Inventory &getPredefinedInventory();


	private:

        void _loadSingleContainedItem(MGFDataReader &reader);

        Inventory mPredefindedInventory;

	};

}

#endif /* INCLUDE_SAS_SASENTITYBASES_H_ */
