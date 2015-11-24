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
	class Entity;

	class WorldManager
	{
	public:

		WorldManager(Root *root);
		~WorldManager();

		void unloadEntities();

		void enterWorld(UID worldUID);

		String getDungeonName();
		std::vector<Entity*> &getEntities();
		void removeEntity(UID entityUID);


	private:

		Root *mRoot;

		String mDungeonName;
		std::vector<Entity*> mEntities;

	};

}


#endif /* INCLUDE_WORLDMANAGER_H_ */
