/*
 * Player.h
 *
 *  Created on: Nov 2, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_PLAYER_H_
#define INCLUDE_PLAYER_H_

#include <IActor.h>
#include "Types.h"
#include "Inventory.h"

namespace Stoneship
{

	class Player : public IActor
	{
	public:

		Player();

		Inventory &getInventory();


	private:

		Inventory mInventory;

	};

}


#endif /* INCLUDE_PLAYER_H_ */
