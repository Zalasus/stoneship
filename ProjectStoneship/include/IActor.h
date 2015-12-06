/*
 * Actor.h
 *
 *  Created on: Nov 16, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_IACTOR_H_
#define INCLUDE_IACTOR_H_

namespace Stoneship
{

	class Inventory;

	class IActor
	{
	public:

		virtual ~IActor();

		virtual Inventory &getInventory() = 0;

	};

}


#endif /* INCLUDE_IACTOR_H_ */
