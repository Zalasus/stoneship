/*
 * Actor.h
 *
 *  Created on: Nov 16, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_ACTOR_H_
#define INCLUDE_ACTOR_H_

namespace Stoneship
{

	class Inventory;

	class Actor
	{
	public:

		virtual ~Actor();

		virtual Inventory &getInventory() = 0;

	};

}


#endif /* INCLUDE_ACTOR_H_ */
