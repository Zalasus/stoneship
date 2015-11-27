/*
 * Entity.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include "Types.h"
#include "Record.h"

namespace Stoneship
{

    class EntityBase;

    class EntityPlaced : public EntityBase
    {

    };

	class Entity : public RecordLoadable
	{
	public:

		enum EntityType
		{
			ENTITYTYPE_DEFAULT,
			ENTITYTYPE_WORLD,
			ENTITYTYPE_ITEM
		};

		Entity(UID uid, EntityBase *base);
		virtual ~Entity();

		virtual EntityType getEntityType() = 0;

		UID getUID() const;
		EntityBase *getBase() const;

	private:

		UID mUID;
		EntityBase *mBase;

	};


	class WorldManager;

	class WorldEntity : public Entity
	{
	public:
		WorldEntity(UID uidOfEntity, EntityBase *base, WorldManager *manager); //uidOfEntity is the UID of the reference, not the referenced base!!!
		WorldEntity(const WorldEntity &e) = delete; //don't copy me!
		~WorldEntity();

		virtual EntityType getEntityType() { return ENTITYTYPE_WORLD;}

        virtual void loadFromRecord(RecordAccessor rec);

		void remove();
		void setHidden(bool hidden);
		bool isHidden() const;

	private:

		WorldManager *mWorldManager;

		bool mHidden;

	};


	class ItemEntity : public WorldEntity
	{
	public:
	    ItemEntity(UID uidOfEntity, EntityBase *base, WorldManager *manager); //uidOfEntity is the UID of the reference, not the referenced base!!!
	    ItemEntity(const ItemEntity &e) = delete; //don't copy me!
        ~ItemEntity();

        virtual EntityType getEntityType() { return ENTITYTYPE_ITEM;}

        virtual void loadFromRecord(RecordAccessor rec);

        uint32_t getCount() const;
        void setCount(uint32_t count);


	private:

        uint32_t mCount;
	};

}


#endif /* INCLUDE_ENTITY_H_ */
