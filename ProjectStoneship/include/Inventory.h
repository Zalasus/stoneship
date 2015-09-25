/*
 * Inventory.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_INVENTORY_H_
#define INCLUDE_INVENTORY_H_

#include <vector>

#include "ItemBase.h"


namespace Stoneship
{

	struct ItemStack
	{
		ItemStack(ItemBase *pItemBase, uint32_t pCount);

		ItemBase *itemBase;
		uint32_t count;
	};

	class Inventory
	{
	public:

		Inventory(uint32_t slots);

		uint32_t getSlots() const;
		uint32_t getUsedSlots();
		inline uint32_t getFreeSlots() { return getSlots() - getUsedSlots(); };

		std::vector<ItemStack> &getItems();

		bool addItem(ItemBase *itemBase, uint32_t count = 1);
		bool removeItem(uint32_t index, uint32_t count = 1);

	private:

		uint32_t mSlots;
		std::vector<ItemStack> mItems;
	};

}


#endif /* INCLUDE_INVENTORY_H_ */
