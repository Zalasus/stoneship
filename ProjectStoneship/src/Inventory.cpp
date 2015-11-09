/*
 * Inventory.cpp
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#include "Inventory.h"

namespace Stoneship
{


	ItemStack::ItemStack(ItemBase *pItemBase, uint32_t pCount)
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

	ItemBase *ItemStack::getItemBase() const
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

	uint32_t Inventory::getSlots() const
	{
		return mSlots;
	}

	uint32_t Inventory::getUsedSlots()
	{
		uint32_t slotsUsed = 0;

		for(uint32_t i = 0; i < mItems.size(); ++i)
		{
			slotsUsed += mItems[i].getItemBase()->getSlots();
		}

		return slotsUsed;
	}

	std::vector<ItemStack> &Inventory::getItems()
	{
		return mItems;
	}

	bool Inventory::addItem(ItemBase *itemBase, uint32_t count)
	{
		if(!itemBase->isNotStackable()) //is item stackable?
		{
			//already got a stack of that item?
			auto it = mItems.begin();
			while(it != mItems.end())
			{
				if(it->getItemBase() == itemBase)
				{
					it->setCount(it->getCount() + count); //yes, increase count

					return true;
				}

				++it;
			}
		}

		//create new stack if enough slots free
		if(getFreeSlots() >= itemBase->getSlots())
		{
			mItems.push_back(ItemStack(itemBase, count));

			return true;
		}

		return false;
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
