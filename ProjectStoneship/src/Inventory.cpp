/*
 * Inventory.cpp
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#include <IEntityBaseItem.h>
#include "Inventory.h"

namespace Stoneship
{


	ItemStack::ItemStack(IEntityBaseItem *pItemBase, uint32_t pCount)
	: mItemBase(pItemBase),
	  mCount(pCount)
	{
		if(mItemBase != nullptr)
		{
			mItemBase->mUserCount++;
		}
	}

	ItemStack::ItemStack(const ItemStack &s)
	: mItemBase(s.getItemBase()),
	  mCount(s.getCount())
	{
		if(mItemBase != nullptr)
		{
			mItemBase->mUserCount++;
		}
	}

	ItemStack::~ItemStack()
	{
		if(mItemBase != nullptr && mItemBase->mUserCount > 0)
		{
			mItemBase->mUserCount--;
		}
	}

	IEntityBaseItem *ItemStack::getItemBase() const
	{
		return mItemBase;
	}

	uint32_t ItemStack::getCount() const
	{
		return mCount;
	}

	void ItemStack::setCount(uint32_t count)
	{
		mCount = count;
	}



	Inventory::Inventory(uint32_t slots)
	: mSlots(slots)
	{
	}

	uint32_t Inventory::getSlotCount() const
	{
		return mSlots;
	}

	void Inventory::setSlotCount(uint32_t i)
	{
	    mSlots = i;
	}

	uint32_t Inventory::getUsedSlotCount()
	{
		uint32_t slotsUsed = 0;

		for(uint32_t i = 0; i < mItems.size(); ++i)
		{
			slotsUsed += mItems[i].getItemBase()->getOccupyingSlots();
		}

		return slotsUsed;
	}

	const Inventory::ItemVector &Inventory::getItems() const
	{
		return mItems;
	}

	void Inventory::copyItems(const Inventory &inv)
	{
	    const ItemVector &iv = inv.getItems();

	    for(uint32_t i = 0; i < iv.size(); ++i)
	    {
	        addItem(iv[i]);
	    }
	}

	void Inventory::clear()
	{
	    mItems.clear();
	}

	uint32_t Inventory::addItem(IEntityBase *base, uint32_t count)
	{
		uint32_t countRemaining = count;

		IEntityBaseItem *itemBase = dynamic_cast<IEntityBaseItem*>(base); // TODO: buähhh

		if(itemBase == nullptr)
		{
			STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Can't add non-item base type " + base->getBaseName() + " to inventory");
		}

		if(itemBase->isStackable()) //is item stackable?
		{
			//already got a stack of that item?
			auto it = mItems.begin();
			while(it != mItems.end() && countRemaining > 0)
			{
				if(it->getItemBase() == itemBase)
				{
					//yes, we already have a stack

					uint32_t oldStackSize = it->getCount();
					uint32_t newStackSize = oldStackSize + countRemaining;

					//is stack size limited and would writing new count exceed the limit?
					if((itemBase->getMaxStackSize() > 0) && (newStackSize > itemBase->getMaxStackSize()))
					{
						//set count to maximum
						newStackSize = itemBase->getMaxStackSize();
					}

					it->setCount(newStackSize);

					countRemaining -= newStackSize - oldStackSize;

				}

				++it;
			}
		}

		while(countRemaining > 0)
		{
			uint32_t newStackSize = countRemaining;

			//is stack size limited and would current stack size exceed maximum?
			if((itemBase->getMaxStackSize() > 0) && (countRemaining > itemBase->getMaxStackSize()))
			{
				//yes, limit this stack to maximum. we'll process the remaining items in the next iteration

				newStackSize = itemBase->getMaxStackSize();
			}

			//create new stack if enough slots free
			if(getFreeSlotCount() >= itemBase->getOccupyingSlots())
			{
				mItems.push_back(ItemStack(itemBase, newStackSize));
				countRemaining -= newStackSize;

			}else
			{
				//can't add any more stacks. we are done here
				break;
			}
		}

		return count - countRemaining;
	}

	uint32_t Inventory::addItem(const ItemStack &stack)
	{
		return addItem(stack.getItemBase(), stack.getCount());
	}

	bool Inventory::removeItem(uint32_t index, uint32_t count)
	{
		if(index < mItems.size())
		{
			if((mItems[index].getCount() - count) <= 0) //would stack be empty if we removed given amount of items?
			{
				mItems.erase(mItems.begin() + index); //yes, remove stack

			}else
			{
				mItems[index].setCount(mItems[index].getCount() - count); //no, just decrease amount
			}

			return true;
		}

		return false;
	}


}
