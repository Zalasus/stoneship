/*
 * Inventory.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_INVENTORY_H_
#define INCLUDE_INVENTORY_H_

#include <vector>

#include "Types.h"

namespace Stoneship
{

    class IEntityBaseItem;

	class ItemStack
	{
	public:
		ItemStack(IEntityBaseItem *pItemBase, uint32_t pCount);
		ItemStack(const ItemStack &s);
		~ItemStack();

		IEntityBaseItem *getItemBase() const;
		uint32_t getCount() const;
		void setCount(uint32_t count);


	private:
		IEntityBaseItem *mItemBase;
		uint32_t mCount;
	};

	class IEntityBase;

	class Inventory
	{
	public:

	    typedef std::vector<ItemStack> ItemVector;

		Inventory(uint32_t slots);

		uint32_t getSlotCount() const;
		void setSlotCount(uint32_t i);
		uint32_t getUsedSlotCount();
		inline uint32_t getFreeSlotCount() { return getSlotCount() - getUsedSlotCount(); };

		const ItemVector &getItems() const;

		/**
		 * Copies all items from inv and adds them to this Inventory. Items already contained are incremented if needed
		 * and are not affected otherwise.
		 */
		void copyItems(const Inventory &inv);

		/**
		 * Removes all contained items.
		 */
		void clear();

		/**
		 * @returns The amount of items that effectively were added to the inventory. Equal to count if enough room for all elements was available.
		 */
		uint32_t addItem(IEntityBase *base, uint32_t count = 1);
		uint32_t addItem(const ItemStack &stack);

		bool removeItem(uint32_t index, uint32_t count = 1);

	private:

		uint32_t mSlots;
		ItemVector mItems;
	};

}


#endif /* INCLUDE_INVENTORY_H_ */
