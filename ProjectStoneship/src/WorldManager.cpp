/*
 * WorldManager.cpp
 *
 *  Created on: 22.09.2015
 *      Author: Zalasus
 */

#include "WorldManager.h"

#include "StoneshipException.h"
#include "MasterGameFile.h"

#include <iostream>

namespace Stoneship
{

	WorldManager::WorldManager(MasterGameFileManager &mgfManager, EntityManager &entityManager)
	: mMGFManager(mgfManager),
	  mEntityManager(entityManager)
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

		RecordAccessor worldRecord = mMGFManager.getRecord(worldUID, Record::TYPE_DUNGEON); //only allow dungeons for now

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

			EntityBase *base = mEntityManager.getBase(baseUID, baseType);
			if(base == nullptr)
			{
				throw StoneshipException("Referenced base not found");
			}

			mEntities.push_back(new Entity(UID(entityRecord.getGameFile()->getOrdinal(), entityRecord.getHeader().id), base));

			if(i < entityGroup.getHeader().recordCount - 1)
			{
				entityRecord = entityRecord.getNextRecord();
			}
		}
	}

	String WorldManager::getDungeonName()
	{
		return mDungeonName;
	}

	std::vector<Entity*> &WorldManager::getEntities()
	{
		return mEntities;
	}

}


