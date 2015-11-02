/*
 * EntityManager.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "EntityManager.h"

#include <utility>

#include "MasterGameFileManager.h"
#include "StoneshipException.h"

namespace Stoneship
{

	EntityManager::EntityManager(MasterGameFileManager &mgfManager)
	: mMGFManager(mgfManager)
	{
	}

	EntityManager::~EntityManager()
	{
		for(uint32_t i = 0; i < mBaseCache.size(); ++i)
		{
			delete mBaseCache[i];
		}
	}

	EntityBase *EntityManager::getBase(const UID &uid, Record::Type type)
	{
		/*auto search = mBaseCache.find(uid.toUInt64());
		if(search != mBaseCache.end())
		{
			return search->second;
		}*/

		//TODO: Make search for cached elements more efficient, but beware memory leak problem of unordered_map
		for(uint32_t i = 0; i < mBaseCache.size(); ++i)
		{
			if(mBaseCache[i]->getUID() == uid)
			{
				return mBaseCache[i];
			}
		}


		// base object is not yet cached. aquire it.

		RecordAccessor rec = mMGFManager.getRecord(uid, type);

		EntityBaseFactory *factory = EntityBaseFactory::getFactoryForRecordType(rec.getHeader().type);
		if(factory == nullptr)
		{
			throw StoneshipException(String("Record type ") + rec.getHeader().type + " does not match any registered Entity Base types.");
		}

		EntityBase *base = factory->createEntityBase(uid);
		base->load(rec);

		// store new base in cache
		//std::pair<uint64_t, EntityBase*> basePair(uid.toUInt64(), base);
		//mBaseCache.insert(basePair);
		mBaseCache.push_back(base);

		return base;
	}

}
