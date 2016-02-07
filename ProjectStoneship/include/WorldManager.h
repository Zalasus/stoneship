/*
 * WorldManager.h
 *
 *  Created on: 22.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_WORLDMANAGER_H_
#define INCLUDE_WORLDMANAGER_H_

#include <vector>

#include "Types.h"
#include "String.h"
#include "Record.h"

namespace Stoneship
{

	class Root;
	class IEntity;
	class IWorld;
	class MGFDataWriter;

	class WorldManager
	{
	public:

		WorldManager(Root *root);
		~WorldManager();

		void enterWorld(UID worldUID);
		IWorld *getCurrentWorld();
		uint32_t storeWorldCache(MGFDataWriter &writer);

	private:

		Root *mRoot;
		std::vector<IWorld*> mWorldCache;
		IWorld *mCurrentWorld;

	};

}


#endif /* INCLUDE_WORLDMANAGER_H_ */
