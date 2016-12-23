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
	class EventPipeline;
	class IEntity;

	class IEntityBaseItem : public IEntityBaseWorld
	{
	private:

	    // lazy-ass class definition
        struct InventoryData : public SubrecordFieldS
        {
            InventoryData(RecordReflector *r) : SubrecordFieldS(r), mFlags(0), mSlotCount(0), mMaxStackSize(0) {};

            // override SubrecordFieldS
            virtual void write(MGFDataWriter &writer) { writer << mFlags << mSlotCount << mMaxStackSize; }
            virtual void read(MGFDataReader &reader) { reader >> mFlags >> mSlotCount >> mMaxStackSize >> MGFDataReader::endr; }
            Record::Subtype getSubtype() const { return Record::SUBTYPE_INVENTORY; }
            SubrecordHeader::SizeType getPredictedDataSize() const { return sizeof(*this); }

            uint8_t getFlags() const { return mFlags; }
            void setFlags(uint8_t i) { mFlags = i; setDirty(true); }
            uint8_t getSlotCount() const { return mSlotCount; };
            void setSlotCount(uint8_t i) { mSlotCount = i; setDirty(true); }
            uint32_t getMaxStackSize() const { return mMaxStackSize; };
            void setMaxStackSize(uint32_t i) { mMaxStackSize = i; setDirty(true); }

        private:

            uint8_t mFlags;
            uint8_t mSlotCount;
            uint32_t mMaxStackSize;
        };


	public:

        IEntityBaseItem(UID uid);
		virtual ~IEntityBaseItem();

		// override IEntityBase
		virtual IEntity *createEntity(UID uid);

		// override IEntityBaseWorld
        virtual bool canInteract() const; // true by default
        virtual bool onInteract(IEntity *entity, IActor *actor, EventPipeline *pipeline); // if not overridden, this creates a pickup event by default

        // override RecordReflector
        virtual bool mustStore(SubrecordFieldS *field);
        virtual bool mustLoad(SubrecordFieldS *field);

        // interface
		virtual bool onUse(ItemStack *stack, IActor *a) = 0;

		String getDisplayName() const;
		void setDisplayName(const String &s);
        String getDescription() const;
        void setDescription(const String &s);
        uint32_t getValue() const;
        void setValue(uint32_t i);
        uint8_t getOccupyingSlots() const;
        void setOccupyingSlots(uint8_t i);
        uint32_t getMaxStackSize() const;
        void setMaxStackSize(uint8_t i);
        bool isStackable() const;
        String getIconFile() const;
        void setIconFile(const String &s);
        UID getIdentifiedUID() const;
        void setIdentifiedUID(UID id);
        uint8_t getFlags() const;
        void setFlags(uint8_t i);
        inline bool isEssential() const { return mInventory.getFlags() & FLAGS_IS_ESSENTIAL; }
        inline bool isCurrency() const { return mInventory.getFlags() & FLAGS_IS_CURRENCY; }
        inline bool isToBeIdentified() const { return mInventory.getFlags() & FLAGS_IS_UNIDENTIFIED; }
        inline bool isUnique() const { return mInventory.getFlags() & FLAGS_IS_UNIQUE; }


		static const uint8_t FLAGS_IS_ESSENTIAL = 0x01;
		static const uint8_t FLAGS_IS_CURRENCY = 0x02;
		static const uint8_t FLAGS_IS_UNIDENTIFIED = 0x04;
		static const uint8_t FLAGS_IS_UNIQUE = 0x10;


	protected:

		bool _pickupOnInteract(IEntity *entity, IActor *actor, EventPipeline *pipeline);


	private:

		SubrecordField<String> mName;
		SubrecordField<String> mDescription;
		SubrecordField<uint32_t> mValue;
		InventoryData mInventory;
		SubrecordField<String> mIconFile;
		SubrecordField<UID> mIdentified;
	};

}


#endif /* INCLUDE_IENTITYBASEITEM_H_ */
