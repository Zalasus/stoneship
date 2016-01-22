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
	    Logger::info("Allocated entity base " + mUID.toString());
	}

	IEntityBase::~IEntityBase()
	{
	    Logger::info("Deallocated entity base " + mUID.toString());
	}

	UID IEntityBase::getUID() const
	{
		return mUID;
	}

	uint32_t IEntityBase::getUserCount() const
	{
		return mUserCount;
	}

	UID::Ordinal IEntityBase::getCreatedBy()
	{
	    return mUID.ordinal;
	}




	EntityBaseFactory::EntityBaseFactory(Record::Type recordType, EntityBaseAllocatorMethodPtr alloc, const String &baseName)
	: mRecordType(recordType),
	  mAllocator(alloc)
	{
	    if(getFactoryForRecordType(recordType) != nullptr)
	    {
	        Logger::warn("Double registration of Entity Base '" + baseName + "'! Ignoring.");

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
