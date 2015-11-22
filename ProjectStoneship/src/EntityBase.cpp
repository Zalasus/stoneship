/*
 * EntityBase.cpp
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#include "EntityBase.h"

#include "sas/SASEntityBases.h" // provided for base registration


namespace Stoneship
{

	EntityBase::EntityBase()
    : mUserCount(0)
	{
	}

	EntityBase::EntityBase(UID uid)
    : mUID(uid),
	  mUserCount(0)
	{
	}

	EntityBase::~EntityBase()
	{
	}

	UID EntityBase::getUID() const
	{
		return mUID;
	}

	uint32_t EntityBase::getUserCount() const
	{
		return mUserCount;
	}




	EntityBaseFactory::EntityBaseFactory(Record::Type recordType, EntityBaseAllocatorMethodPtr alloc)
	: mRecordType(recordType),
	  mAllocator(alloc)
	{
		smFactories.push_back(this);
	}

	Record::Type EntityBaseFactory::getRecordType() const
	{
		return mRecordType;
	}

	EntityBase *EntityBaseFactory::createEntityBase(UID uid)
	{
		return (mAllocator(uid));
	}


	EntityBaseFactory *EntityBaseFactory::getFactoryForRecordType(Record::Type t)
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

	std::vector<EntityBaseFactory*> EntityBaseFactory::smFactories;


	//for now, all EntityBase registrations need to be put here in order to guarantee the are created AFTER the vector in the factory

	REGISTER_ENTITY_BASE(0x800, EntityBase_Static)
	REGISTER_ENTITY_BASE(0x810, EntityBase_Weapon)
	REGISTER_ENTITY_BASE(0x820, EntityBase_Book)
	REGISTER_ENTITY_BASE(0x821, EntityBase_Stuff)

}
