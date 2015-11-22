/*
 * EntityManager.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "MGFManager.h"
#include "EntityManager.h"

#include <utility>

#include "StoneshipException.h"

namespace Stoneship
{

	EntityManager::EntityManager(Root *root)
	: mRoot(root)
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

		RecordAccessor rec = mRoot->getMGFManager()->getRecordByTypeID(uid, type);

		EntityBaseFactory *factory = EntityBaseFactory::getFactoryForRecordType(rec.getHeader().type);
		if(factory == nullptr)
		{
			STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, String("Record type ") + rec.getHeader().type + " does not match any registered Entity Base types.");
		}

		EntityBase *base = factory->createEntityBase(uid);

		try
		{
			base->loadFromRecord(rec);

		}catch(StoneshipException &e)
		{
			if(e.getType() == StoneshipException::IO_ERROR)
			{
				STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Malformed record or IO error. Could not load entity base.");
			}

			throw;
		}

		//apply modifications. do this for every new record for now. we might think of situations when we don't have to do this in the future
		try
		{
			mRoot->getMGFManager()->applyModifications(base);

		}catch(StoneshipException &e)
		{
			if(e.getType() == StoneshipException::IO_ERROR)
			{
				STONESHIP_EXCEPT(StoneshipException::ENTITY_ERROR, "Malformed modify record or IO error. Could not apply modifications to base.");
			}

			throw;
		}


		// store new base in cache
		//std::pair<uint64_t, EntityBase*> basePair(uid.toUInt64(), base);
		//mBaseCache.insert(basePair);
		mBaseCache.push_back(base);

		return base;
	}

	uint32_t EntityManager::getBaseCacheSize()
	{
		return mBaseCache.size();
	}

	void EntityManager::collectGarbage()
	{
		auto it = mBaseCache.begin();
		while(it != mBaseCache.end())
		{
			if(!(*it)->getUserCount())
			{
				delete (*it);

				it = mBaseCache.erase(it);

			}else
			{
				++it;
			}
		}
	}

}
