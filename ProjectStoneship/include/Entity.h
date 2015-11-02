/*
 * Entity.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include <vector>

#include "Record.h"

#define REGISTER_ENTITY_BASE(recordType, entityBaseClass) static EntityBase* create_ ## entityBaseClass ## _instance(Record::Type r, UID uid) { return new entityBaseClass(r, uid); } static EntityBaseFactory entityBaseClass ## _factory(recordType, & create_ ## entityBaseClass ## _instance);

namespace Stoneship
{

	class EntityBase
	{
	public:

		EntityBase();
		EntityBase(Record::Type recordType, UID uid);
		virtual ~EntityBase();

		Record::Type getRecordType() const;
		UID getUID() const;

		virtual void load(RecordAccessor record) = 0;
		virtual void modify(RecordAccessor record) = 0;

		virtual String getBaseName() = 0;


	private:

		Record::Type mRecordType;
		UID mUID;
	};


	class EntityBaseFactory
	{
	public:

		typedef EntityBase* (*EntityBaseAllocatorMethodPtr)(Record::Type, UID);


		EntityBaseFactory(Record::Type recordType, EntityBaseAllocatorMethodPtr alloc);

		Record::Type getRecordType() const;
		EntityBase *createEntityBase(UID uid);


		static EntityBaseFactory *getFactoryForRecordType(Record::Type t);


	private:

		Record::Type mRecordType;
		EntityBaseAllocatorMethodPtr mAllocator;

		static std::vector<EntityBaseFactory*> smFactories;
	};



	class Entity
	{
	public:
		Entity(UID uid, EntityBase *base);

		UID getUID();
		EntityBase *getBase();


	private:

		UID mUID;
		EntityBase *mBase;

	};

}


#endif /* INCLUDE_ENTITY_H_ */
