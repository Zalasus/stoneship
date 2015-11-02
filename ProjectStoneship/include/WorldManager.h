/*
 * WorldManager.h
 *
 *  Created on: 22.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_WORLDMANAGER_H_
#define INCLUDE_WORLDMANAGER_H_

#include "MasterGameFileManager.h"
#include "EntityManager.h"

namespace Stoneship
{

	class WorldManager
	{
	public:

		WorldManager(MasterGameFileManager &mgfManager, EntityManager &entityManager);
		~WorldManager();

		void unloadEntities();

		void enterWorld(UID worldUID);

		String getDungeonName();
		std::vector<Entity*> &getEntities();


	private:

		MasterGameFileManager &mMGFManager;
		EntityManager &mEntityManager;


		String mDungeonName;
		std::vector<Entity*> mEntities;

	};

}


#endif /* INCLUDE_WORLDMANAGER_H_ */
