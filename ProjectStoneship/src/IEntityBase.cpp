/*
 * EntityBase.cpp
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#include "IEntityBase.h"

#include "Logger.h"

namespace Stoneship
{

	IEntityBase::IEntityBase()
    : mUserCount(0)
	{
	}

	IEntityBase::IEntityBase(UID uid)
    : mUID(uid),
	  mUserCount(0)
	{
	}

	IEntityBase::~IEntityBase()
	{
	}

	UID IEntityBase::getUID() const
	{
		return mUID;
	}

	uint32_t IEntityBase::getUserCount() const
	{
		return mUserCount;
	}




	EntityBaseFactory::EntityBaseFactory(Record::Type recordType, EntityBaseAllocatorMethodPtr alloc, const String &baseName)
	: mRecordType(recordType),
	  mAllocator(alloc)
	{
	    if(getFactoryForRecordType(recordType) != nullptr)
	    {
	        Logger::warn("Double registration for Entity Base '" + baseName + "'! Ignoring.");

	    }else
	    {
	        Logger::info("Registered Entity Base type '" + baseName + "'");

	        smFactories.push_back(this);
	    }
	}

	Record::Type EntityBaseFactory::getRecordType() const
	{
		return mRecordType;
	}

	IEntityBase *EntityBaseFactory::createEntityBase(UID uid)
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

}
