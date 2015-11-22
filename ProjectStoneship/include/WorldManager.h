/*
 * WorldManager.h
 *
 *  Created on: 22.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_WORLDMANAGER_H_
#define INCLUDE_WORLDMANAGER_H_

#include "MGFManager.h"
#include "EntityManager.h"

namespace Stoneship
{

	class WorldManager
	{
	public:

		WorldManager(Root *root);
		~WorldManager();

		void unloadEntities();

		void enterWorld(UID worldUID);

		String getDungeonName();
		std::vector<WorldEntity*> &getEntities();
		void removeEntity(UID entityUID);


	private:

		Root *mRoot;

		String mDungeonName;
		std::vector<WorldEntity*> mEntities;

	};

}


#endif /* INCLUDE_WORLDMANAGER_H_ */
