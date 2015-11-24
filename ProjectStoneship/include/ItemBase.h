/*
 * Item.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_ITEMBASE_H_
#define INCLUDE_ITEMBASE_H_

#include "Types.h"
#include "String.h"
#include "EntityBase.h"
#include "WorldEntityBase.h"
#include "Record.h"

namespace Stoneship
{
	class ItemStack;
	class Actor;
	class Entity;

	class ItemBase : public WorldEntityBase
	{
	public:

		virtual ~ItemBase();

		String getDisplayName() const;
		String getDescription() const;
		uint32_t getValue() const;
		uint8_t getSlots() const;
		uint32_t getMaxStackSize() const;
		bool isStackable() const;
		String getIconFile() const;
		UID getIdentifiedUID() const;

		void loadFromRecord(RecordAccessor record);

		virtual bool onUse(ItemStack *stack, Actor *a) = 0;

		inline bool isEssential() const { return mFlags & FLAGS_IS_ESSENTIAL; }
		inline bool isCurrency() const { return mFlags & FLAGS_IS_CURRENCY; }
		inline bool isUnidentified() const { return mFlags & FLAGS_IS_UNIDENTIFIED; }
		inline bool isUnique() const { return mFlags & FLAGS_IS_UNIQUE; }

		static const uint8_t FLAGS_IS_ESSENTIAL = 0x01;
		static const uint8_t FLAGS_IS_CURRENCY = 0x02;
		static const uint8_t FLAGS_IS_UNIDENTIFIED = 0x04;
		static const uint8_t FLAGS_IS_UNIQUE = 0x10;


	protected:

		ItemBase(UID uid);

		bool _pickupOnInteract(Entity *entity, Actor *actor);

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


#endif /* INCLUDE_ITEMBASE_H_ */
