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
    class IEntityBase;

	class EntityManager
	{
	public:

		EntityManager(Root *root);
		~EntityManager();

		IEntityBase *getBase(const UID &uid, Record::Type type = Record::TYPE_LOOKUP_ALL);
		IEntityBase *manageBase(IEntityBase *base);

		uint32_t getBaseCacheSize();
		void collectGarbage();

		void storeCache(MGFDataWriter &writer);
		void storeCacheMods(MGFDataWriter &writer);

	private:

		Root *mRoot;

		std::vector<IEntityBase*> mBaseCache;
	};

}


#endif /* INCLUDE_ENTITYMANAGER_H_ */
