/*
 * EntityBase.h
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_IENTITYBASE_H_
#define INCLUDE_IENTITYBASE_H_

#include <RecordReflector.h>
#include <vector>

#include "Types.h"
#include "String.h"
#include "Record.h"

//These have to be used to frame the block in which entities are registered
#define REGISTER_ENTITY_BEGIN
#define REGISTER_ENTITY_END ;
#define REGISTER_ENTITY_BASE(recordType, entityBaseClass, baseName, preloaded) static IEntityBase* create_ ## entityBaseClass ## _instance(UID uid) { return new entityBaseClass(uid); } static EntityBaseFactory entityBaseClass ## _factory(recordType, & create_ ## entityBaseClass ## _instance, # baseName, preloaded);

namespace Stoneship
{

    class IEntity;

	class IEntityBase : public RecordReflector
	{
	public:

		friend class IEntity;
        friend class ItemStack; //TODO: I'd prefer a solution in which ItemStack just uses an Entity instance to do GC

        // TODO: Maybe create a bitmask type for this
		typedef uint32_t BaseType;
		static const BaseType BASETYPE_ITEM = 1;
		static const BaseType BASETYPE_WORLD = 2;
		static const BaseType BASETYPE_SPECIAL = 4;


		IEntityBase();
		IEntityBase(UID uid);
		virtual ~IEntityBase();

		// overrides RecordReflector
		virtual UID getCreatedUID() const;

		// interface
		virtual BaseType getBaseType() const = 0;
		virtual String getBaseName() const = 0;
		virtual IEntity *createEntity(UID entityUID) = 0;

		UID getUID() const;
        uint32_t getUserCount() const;

	private:

		UID mUID;
		uint32_t mUserCount;
	};


	class EntityBaseFactory
	{
	public:

		typedef IEntityBase* (*EntityBaseAllocatorMethodPtr)(UID);


		EntityBaseFactory(Record::Type recordType, EntityBaseAllocatorMethodPtr alloc, const String &baseName, bool preloaded);

		Record::Type getRecordType() const;
		bool isPreloaded() const;
		IEntityBase *createEntityBase(UID uid);


		static EntityBaseFactory *getFactoryForRecordType(Record::Type t);

	private:

		static std::vector<EntityBaseFactory*> &getFactoryVector();

		Record::Type mRecordType;
		bool mPreloaded;
		EntityBaseAllocatorMethodPtr mAllocator;
	};

}



#endif /* INCLUDE_IENTITYBASE_H_ */
