/*
 * WorldManager.cpp
 *
 *  Created on: 22.09.2015
 *      Author: Zalasus
 */

#include "WorldManager.h"

#include "StoneshipException.h"
#include "MasterGameFile.h"

namespace Stoneship
{

	WorldManager::WorldManager(MasterGameFileManager &mgfManager, EntityManager &entityManager)
	: mMGFManager(mgfManager),
	  mEntityManager(entityManager)
	{
	}

	void WorldManager::enterWorld(UID worldUID)
	{
		RecordAccessor worldRecord = mMGFManager.getRecord(worldUID, Record::TYPE_DUNGEON); //only allow dungeons for now

		mDungeonName = worldRecord.getReaderForSubrecord(Record::SUBTYPE_DATA).readBString();

		RecordAccessor entityGroup = worldRecord.getNextRecord();
		if(entityGroup.getHeader().type != Record::TYPE_GROUP)
		{
			return; //no entities
		}

		while(entityGroup.getReader().bytesRemainingInUnit())
		{
			RecordAccessor entityRecord = entityGroup.getChildRecord();

			UID baseUID;
			entityRecord.getReaderForSubrecord(0x01).readStruct(baseUID);

			EntityBase *base = mEntityManager.getBase(baseUID);
			if(base == nullptr)
			{
				throw StoneshipException("Referenced base not found");
			}

			mEntities.push_back(new Entity(UID(entityRecord.getGameFile().getOrdinal(), entityRecord.getHeader().id), base));
		}
	}

}


