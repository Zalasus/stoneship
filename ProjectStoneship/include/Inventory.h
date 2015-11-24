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

    class ItemBase;

	class ItemStack
	{
	public:
		ItemStack(ItemBase *pItemBase, uint32_t pCount);
		ItemStack(const ItemStack &s);
		~ItemStack();

		ItemBase *getItemBase() const;
		uint32_t getCount() const;
		void setCount(uint32_t count);


	private:
		ItemBase *mItemBase;
		uint32_t mCount;
	};


	class EntityBase;

	class Inventory
	{
	public:

		Inventory(uint32_t slots);

		uint32_t getSlots() const;
		uint32_t getUsedSlots();
		inline uint32_t getFreeSlots() { return getSlots() - getUsedSlots(); };

		std::vector<ItemStack> &getItems();

		/**
		 * @returns The actual amount of items that were added to the inventory
		 */
		uint32_t addItem(EntityBase *base, uint32_t count = 1);
		uint32_t addItem(const ItemStack &stack);

		bool removeItem(uint32_t index, uint32_t count = 1);

	private:

		uint32_t mSlots;
		std::vector<ItemStack> mItems;
	};

}


#endif /* INCLUDE_INVENTORY_H_ */
