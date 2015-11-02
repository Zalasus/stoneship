/*
 * Item.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_ITEMBASE_H_
#define INCLUDE_ITEMBASE_H_

#include "Util.h"
#include "String.h"
#include "Entity.h"
#include "Record.h"
#include "WorldObjectBase.h"

namespace Stoneship
{
	class ItemStack;

	class ItemBase : public WorldObjectBase
	{
	public:

		String getDisplayName() const;
		String getDescription() const;
		uint32_t getValue() const;
		uint8_t getSlots() const;
		String getIconFile() const;
		UID getIdentifiedUID() const;

		virtual void load(RecordAccessor record);

		virtual bool onUse(ItemStack &stack) = 0;


		inline bool isEssential() const { return mFlags & FLAGS_ESSENTIAL; }
		inline bool isCurrency() const { return mFlags & FLAGS_CURRENCY; }
		inline bool isUnidentified() const { return mFlags & FLAGS_UNIDENTIFIED; }
		inline bool isNotStackable() const { return mFlags & FLAGS_DOES_NOT_STACK; }

		static const uint8_t FLAGS_ESSENTIAL = 0x01;
		static const uint8_t FLAGS_CURRENCY = 0x02;
		static const uint8_t FLAGS_UNIDENTIFIED = 0x04;
		static const uint8_t FLAGS_DOES_NOT_STACK = 0x08;


	protected:

		ItemBase(Record::Type recordType, UID uid);


	private:

		uint8_t mFlags;
		String mName;
		String mDescription;
		uint32_t mValue;
		uint8_t mSlots;
		String mIconFile;
		UID mIdentified;
	};

}


#endif /* INCLUDE_ITEMBASE_H_ */
