/*
 * EntityBase.cpp
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#include "Entity.h"
#include "sas/SASEntities.h" // provided for base registration


namespace Stoneship
{

    Entity::Entity(UID uid, Entity *parent)
    : mUID(uid),
      mParent(parent),
	  mUserCount(0)
	{
	}

    Entity::~Entity()
	{
	}

	UID Entity::getUID() const
	{
		return mUID;
	}

	uint32_t Entity::getUserCount() const
	{
		return mUserCount;
	}




	EntityFactory::EntityFactory(Record::Type recordType, EntityAllocatorMethodPtr alloc)
	: mRecordType(recordType),
	  mAllocator(alloc)
	{
		smFactories.push_back(this);
	}

	Record::Type EntityFactory::getRecordType() const
	{
		return mRecordType;
	}

	EntityBase *EntityFactory::createEntity(UID uid)
	{
		return (mAllocator(uid));
	}


	EntityFactory *EntityFactory::getFactoryForRecordType(Record::Type t)
	{
		for(uint32_t i = 0; i < smFactories.size(); ++i)
		{
			if(smFactories[i]->getRecordType() == t)
			{
				return smFactories[i];
			}
		}

		return nullptr;
	}

	std::vector<EntityFactory*> EntityFactory::smFactories;


	//for now, all EntityBase registrations need to be put here in order to guarantee the are created AFTER the vector in the factory

	REGISTER_ENTITY(0x800, Entity_Static)
	REGISTER_ENTITY(0x810, Entity_Weapon)
	REGISTER_ENTITY(0x820, Entity_Book)
	REGISTER_ENTITY(0x821, Entity_Stuff)

}
