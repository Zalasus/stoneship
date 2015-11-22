/*
 * Player.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: zalasus
 */

#include "Player.h"

namespace Stoneship
{

	Player::Player()
	: mInventory(20)
	{
	}

	Inventory &Player::getInventory()
	{
		return mInventory;
	}

}


