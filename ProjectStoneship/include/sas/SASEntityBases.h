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

	class EntityBase_Static : public IEntityBaseWorld
	{
	public:
		EntityBase_Static(UID uid);

		// override IEntityBase
		Record::Type getRecordType() const {return 0x800;}
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

		// override IEntityBase
		Record::Type getRecordType() const {return 0x820;}
		IEntityBase::BaseType getBaseType() const {return BASETYPE_WORLD | BASETYPE_ITEM;}
		String getBaseName() const {return "Book";}

		// override IEntityBaseItem
		bool onUse(ItemStack *stack, IActor *actor);

        // interact events from IEntityBaseWorld not overridden. they create pickup event by default

		String getText() const;
		void setText(const String &text);


	private:

		SubrecordField<String, Record::SUBTYPE_TEXT> mText;

	};

	class EntityBase_Weapon : public IEntityBaseItem
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

		// override IEntityBase
		Record::Type getRecordType() const {return 0x810;}
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

		WeaponType mWeaponType;
		uint32_t mDamage;
		uint32_t mDurability;
		uint32_t mReach;

	};

	class EntityBase_Stuff : public IEntityBaseItem
	{
	public:

		EntityBase_Stuff(UID uid);

		// override IEntityBase
		Record::Type getRecordType() const {return 0x821;}
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

	    // override IEntityBase
	    Record::Type getRecordType() const {return 0x82A;}
        IEntityBase::BaseType getBaseType() const {return BASETYPE_WORLD;}
        String getBaseName() const {return "Container";}
        IEntity *createEntity(UID entityUID); // this Base needs a custom Entity that has an Inventory

        // override IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record, Record::ModifyType modType);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeToModifyRecord(RecordBuilder &record);

        // override IEntityBaseWorld
        bool canInteract() const;
        bool onInteract(IEntity *entity, IActor *actor);

        const Inventory &getInventory() const;


	private:

        void _loadSingleContainedItem(MGFDataReader &reader);

        Inventory mPredefindedInventory;

	};

}

#endif /* INCLUDE_SAS_SASENTITYBASES_H_ */
