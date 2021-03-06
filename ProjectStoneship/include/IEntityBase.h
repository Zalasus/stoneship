/*
 * EntityBase.h
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_IENTITYBASE_H_
#define INCLUDE_IENTITYBASE_H_

#include <vector>

#include "Types.h"
#include "String.h"
#include "Record.h"
#include "RecordReflector.h"

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


        IEntityBase();
		IEntityBase(UID uid);
		virtual ~IEntityBase();

		// overrides RecordReflector
		virtual UID getCreatedUID() const;

		// interface
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
		String getBaseName() const;
		bool isPreloaded() const;
		IEntityBase *createEntityBase(UID uid);


		static EntityBaseFactory *getFactoryForRecordType(Record::Type t);
		static uint32_t getRegisteredFactoryCount();
        static void logRegisteredBases();


	private:

		static std::vector<EntityBaseFactory*> &getFactoryVector();

		Record::Type mRecordType;
		String mBaseName;
		bool mPreloaded;
		EntityBaseAllocatorMethodPtr mAllocator;
	};

}



#endif /* INCLUDE_IENTITYBASE_H_ */
