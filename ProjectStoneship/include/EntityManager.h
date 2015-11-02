/*
 * EntityManager.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_ENTITYMANAGER_H_
#define INCLUDE_ENTITYMANAGER_H_

#include <cstdint>
#include <unordered_map>

#include "Entity.h"
#include "Record.h"


namespace Stoneship
{
	class MasterGameFileManager;

	class EntityManager
	{
	public:

		EntityManager(MasterGameFileManager &mgfManager);
		~EntityManager();

		EntityBase *getBase(const UID &uid, Record::Type type = Record::TYPE_LOOKUP_ALL);

	private:

		MasterGameFileManager &mMGFManager;

		std::vector<EntityBase*> mBaseCache;
		//std::unordered_map<uint64_t, EntityBase*> mBaseCache;
	};

}


#endif /* INCLUDE_ENTITYMANAGER_H_ */
