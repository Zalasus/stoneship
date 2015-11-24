/*
 * EntityManager.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_ENTITYMANAGER_H_
#define INCLUDE_ENTITYMANAGER_H_

#include <vector>

#include "Types.h"
#include "Record.h"

namespace Stoneship
{

    class Root;
    class EntityBase;

	class EntityManager
	{
	public:

		EntityManager(Root *root);
		~EntityManager();

		EntityBase *getBase(const UID &uid, Record::Type type = Record::TYPE_LOOKUP_ALL);

		uint32_t getBaseCacheSize();
		void collectGarbage();


	private:

		Root *mRoot;

		std::vector<EntityBase*> mBaseCache;
		//std::unordered_map<uint64_t, EntityBase*> mBaseCache;
	};

}


#endif /* INCLUDE_ENTITYMANAGER_H_ */
