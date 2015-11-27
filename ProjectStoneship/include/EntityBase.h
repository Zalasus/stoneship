/*
 * EntityBase.h
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_ENTITYBASE_H_
#define INCLUDE_ENTITYBASE_H_

#include <vector>

#include "Types.h"
#include "String.h"
#include "Record.h"

#define REGISTER_ENTITY_BASE(recordType, entityBaseClass) static EntityBase* create_ ## entityBaseClass ## _instance(UID uid) { return new entityBaseClass(uid); } static EntityBaseFactory entityBaseClass ## _factory(recordType, & create_ ## entityBaseClass ## _instance);


namespace Stoneship
{

	class EntityBase : public RecordLoadable
	{
	public:

		friend class Entity;
        friend class ItemStack; //TODO: I'd prefer a solution in which ItemStack just uses an Entity instance to do GC

		typedef uint32_t BaseType;
		static const BaseType BASETYPE_ITEM = 1;
		static const BaseType BASETYPE_WORLD = 2;
		static const BaseType BASETYPE_SPECIAL = 4;

		EntityBase();
		EntityBase(UID uid);
		virtual ~EntityBase();

		virtual Record::Type getRecordType() const = 0;
		virtual BaseType getBaseType() const = 0;
		virtual String getBaseName() const = 0;


		UID getUID() const;

		uint32_t getUserCount() const;

	private:

		UID mUID;

		uint32_t mUserCount;
	};


	class EntityBaseFactory
	{
	public:

		typedef EntityBase* (*EntityBaseAllocatorMethodPtr)(UID);


		EntityBaseFactory(Record::Type recordType, EntityBaseAllocatorMethodPtr alloc);

		Record::Type getRecordType() const;
		EntityBase *createEntityBase(UID uid);


		static EntityBaseFactory *getFactoryForRecordType(Record::Type t);


	private:

		Record::Type mRecordType;
		EntityBaseAllocatorMethodPtr mAllocator;

		static std::vector<EntityBaseFactory*> smFactories;
	};

}



#endif /* INCLUDE_ENTITYBASE_H_ */
