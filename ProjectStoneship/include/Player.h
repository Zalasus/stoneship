/*
 * Player.h
 *
 *  Created on: Nov 2, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_PLAYER_H_
#define INCLUDE_PLAYER_H_

#include "Types.h"
#include "Actor.h"
#include "Inventory.h"

namespace Stoneship
{

	class Player : public Actor
	{
	public:

		Player();

		Inventory &getInventory();


	private:

		Inventory mInventory;

	};

}


#endif /* INCLUDE_PLAYER_H_ */
