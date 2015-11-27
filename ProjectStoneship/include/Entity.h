/*
 * Entity.h
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include <vector>

#include "Types.h"
#include "String.h"
#include "Record.h"

#define REGISTER_ENTITY(recordType, entityClass) static Entity* create_ ## entityClass ## _instance(UID uid) { return new entityClass(uid); } static EntityFactory entityClass ## _factory(recordType, & create_ ## entityClass ## _instance);


namespace Stoneship
{

	class Entity : public RecordLoadable
	{
	public:

        friend class ItemStack;

		typedef uint32_t EntityClassType;
		static const EntityClassType BASETYPE_ITEM = 1;
		static const EntityClassType BASETYPE_WORLD = 2;
		static const EntityClassType BASETYPE_SPECIAL = 4;

		Entity(UID uid, Entity *parent = nullptr);
		virtual ~Entity();

		virtual Record::Type getRecordType() const = 0;
		virtual EntityClassType getClassType() const = 0;
		virtual String getSymbolicName() const = 0;


		UID getUID() const;
		Entity *getParent() const;

		uint32_t getUserCount() const;

	private:

		UID mUID;
		Entity *mParent;

		uint32_t mUserCount;
	};


	class EntityFactory
	{
	public:

		typedef Entity* (*EntityAllocatorMethodPtr)(UID);


		EntityFactory(Record::Type recordType, EntityAllocatorMethodPtr alloc);

		Record::Type getRecordType() const;
		EntityBase *createEntity(UID uid);


		static EntityFactory *getFactoryForRecordType(Record::Type t);


	private:

		Record::Type mRecordType;
		EntityAllocatorMethodPtr mAllocator;

		static std::vector<EntityFactory*> smFactories;
	};

}



#endif /* INCLUDE_ENTITY_H_ */
