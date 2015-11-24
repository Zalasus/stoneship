/*
 * WorldManager.cpp
 *
 *  Created on: 22.09.2015
 *      Author: Zalasus
 */

#include "WorldManager.h"

#include <iostream>

#include "Exception.h"
#include "Entity.h"
#include "MasterGameFile.h"
#include "Root.h"
#include "MGFManager.h"
#include "EntityManager.h"

namespace Stoneship
{

	WorldManager::WorldManager(Root *root)
	: mRoot(root)
	{
	}

	WorldManager::~WorldManager()
	{
		unloadEntities();
	}

	void WorldManager::unloadEntities()
	{
		for(uint32_t i = 0 ; i < mEntities.size(); ++i)
		{
			delete mEntities[i];
		}

		mEntities.clear();
	}

	void WorldManager::enterWorld(UID worldUID)
	{
		unloadEntities();

		RecordAccessor worldRecord = mRoot->getMGFManager()->getRecordByTypeID(worldUID, Record::TYPE_DUNGEON); //only allow dungeons for now

		worldRecord.getReaderForSubrecord(Record::SUBTYPE_DATA).readBString(mDungeonName);

		RecordAccessor entityGroup = worldRecord.getNextRecord();
		if(entityGroup.getHeader().type != Record::TYPE_GROUP)
		{
			return; //no entities
		}

		//according to MGF specification, entity group records following dungeons must never be empty
		//anyway, at least getFirstChildRecord() should check the bounds of... wait... it already does. nevermind.

		RecordAccessor entityRecord = entityGroup.getFirstChildRecord();
		for(uint32_t i = 0 ; i < entityGroup.getHeader().recordCount ; ++i)
		{
			UID baseUID;
			Record::Type baseType;
			entityRecord.getReaderForSubrecord(0x01)
					.readStruct(baseUID)
					.readUShort(baseType);

			EntityBase *base = mRoot->getEntityManager()->getBase(baseUID, baseType);
			if(base == nullptr)
			{
				STONESHIP_EXCEPT(StoneshipException::RECORD_NOT_FOUND, "Referenced base not found");
			}

			mEntities.push_back(new WorldEntity(UID(entityRecord.getGameFile()->getOrdinal(), entityRecord.getHeader().id), base, this));

			if(i < entityGroup.getHeader().recordCount - 1)
			{
				entityRecord = entityRecord.getNextRecord();
			}
		}

		mRoot->getEntityManager()->collectGarbage(); //remove all bases that are not used atm
	}

	String WorldManager::getDungeonName()
	{
		return mDungeonName;
	}

	std::vector<Entity*> &WorldManager::getEntities()
	{
		return mEntities;
	}

	void WorldManager::removeEntity(UID entityUID)
	{
		auto it = mEntities.begin();
		while(it != mEntities.end())
		{
			if((*it)->getUID() == entityUID)
			{
				mEntities.erase(it);
				break;
			}

			it++;
		}
	}
}


