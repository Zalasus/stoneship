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

		friend class Entity;
		friend class ItemStack;

		EntityBase();
		EntityBase(Record::Type recordType, UID uid);
		virtual ~EntityBase();

		Record::Type getRecordType() const;
		UID getUID() const;

		virtual void loadFromRecord(RecordAccessor record) = 0;
		virtual void modifyFromRecord(RecordAccessor record) = 0;

		virtual String getBaseName() = 0;

		uint32_t getUserCount() const;

	private:

		Record::Type mRecordType;
		UID mUID;

		uint32_t mUserCount;
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
		Entity(UID uidOfEntity, EntityBase *base); //uidOfEntity is the UID of the reference, not the referenced base!!!
		Entity(const Entity &e) = delete; //don't copy me!
		~Entity();

		UID getUID() const;
		EntityBase *getBase() const;


	private:

		UID mUID;
		EntityBase *mBase;

	};

}


#endif /* INCLUDE_ENTITY_H_ */
