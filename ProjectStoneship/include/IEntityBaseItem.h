/*
 * Item.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_IENTITYBASEITEM_H_
#define INCLUDE_IENTITYBASEITEM_H_

#include "IEntityBaseWorld.h"
#include "IEntityBase.h"
#include "Types.h"
#include "String.h"
#include "Record.h"

namespace Stoneship
{
	class ItemStack;
	class IActor;
	class IEntity;

	class IEntityBaseItem : public IEntityBaseWorld
	{
	public:

        IEntityBaseItem(UID uid);
		virtual ~IEntityBaseItem();

		// override IEntityBase
		virtual IEntity *createEntity(UID uid);

		// override IEntityBaseWorld
        virtual bool canInteract() const; // true by default
        virtual bool onInteract(IEntity *entity, IActor *actor); // if not overridden, this creates a pickup event by default

        // override IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record, Record::ModifyType modType);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeToModifyRecord(RecordBuilder &record);

        // interface
		virtual bool onUse(ItemStack *stack, IActor *a) = 0;

		String getDisplayName() const;
        String getDescription() const;
        uint32_t getValue() const;
        uint8_t getSlots() const;
        uint32_t getMaxStackSize() const;
        bool isStackable() const;
        String getIconFile() const;
        UID getIdentifiedUID() const;
        inline bool isEssential() const { return mFlags & FLAGS_IS_ESSENTIAL; }
        inline bool isCurrency() const { return mFlags & FLAGS_IS_CURRENCY; }
        inline bool isUnidentified() const { return mFlags & FLAGS_IS_UNIDENTIFIED; }
        inline bool isUnique() const { return mFlags & FLAGS_IS_UNIQUE; }

		static const uint8_t FLAGS_IS_ESSENTIAL = 0x01;
		static const uint8_t FLAGS_IS_CURRENCY = 0x02;
		static const uint8_t FLAGS_IS_UNIDENTIFIED = 0x04;
		static const uint8_t FLAGS_IS_UNIQUE = 0x10;


	protected:

		bool _pickupOnInteract(IEntity *entity, IActor *actor);


	private:

		uint8_t mFlags;
		String mName;
		String mDescription;
		uint32_t mValue;
		uint8_t mSlots;
		uint32_t mMaxStackSize;
		String mIconFile;
		UID mIdentified;
	};

}


#endif /* INCLUDE_IENTITYBASEITEM_H_ */
