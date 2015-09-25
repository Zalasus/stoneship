/*
 * Entity.cpp
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#include "Entity.h"

#include "sas/SASEntityBases.h"

namespace Stoneship
{

	EntityBase::EntityBase()
	: mRecordType(0)
	{
	}

	EntityBase::EntityBase(Record::Type recordType, UID uid)
	: mRecordType(recordType),
	  mUID(uid)
	{
	}

	EntityBase::~EntityBase()
	{
	}

	Record::Type EntityBase::getRecordType() const
	{
		return mRecordType;
	}

	UID EntityBase::getUID() const
	{
		return mUID;
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
		return (mAllocator(mRecordType, uid));
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

	REGISTER_ENTITY_BASE(0x810, EntityBase_Weapon)
	REGISTER_ENTITY_BASE(0x820, EntityBase_Book)
	REGISTER_ENTITY_BASE(0x821, EntityBase_Stuff)




	Entity::Entity(UID uid, EntityBase *base)
	: mUID(uid),
	  mBase(base)
	{
	}

	UID Entity::getUID()
	{
		return mUID;
	}

	EntityBase *Entity::getBase()
	{
		return mBase;
	}
}



