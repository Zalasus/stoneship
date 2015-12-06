/*
 * Entity.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_IENTITY_H_
#define INCLUDE_IENTITY_H_

#include "Types.h"
#include "Record.h"

namespace Stoneship
{

    class IEntityBase;
    class IWorld;

	class IEntity : public IRecordLoadable
	{
	public:

		typedef uint32_t EntityType;
		static const EntityType ENTITYTYPE_DEFAULT = 1;
		static const EntityType ENTITYTYPE_WORLD = 2;
		static const EntityType ENTITYTYPE_ITEM = 4;

		IEntity(UID uid, IEntityBase *base);
		virtual ~IEntity();

		virtual EntityType getEntityType() = 0;

		UID getUID() const;
		IEntityBase *getBase() const;

		virtual void spawn(IWorld *w) = 0;
		virtual void despawn() = 0;

	private:

		UID mUID;
		IEntityBase *mBase;

	};


	class WorldManager;

	class EntityWorld : public IEntity
	{
	public:
		EntityWorld(UID uidOfEntity, IEntityBase *base); //uidOfEntity is the UID of the reference, not the referenced base!!!
		EntityWorld(const EntityWorld &e) = delete; //don't copy me!
		~EntityWorld();

		virtual EntityType getEntityType() { return ENTITYTYPE_WORLD;}

        virtual void loadFromRecord(RecordAccessor rec);


        void spawn(IWorld *w);
        void despawn();
		void remove();
		void setHidden(bool hidden);
		bool isHidden() const;

	private:

		IWorld *mWorld;

		bool mHidden;

	};


	class EntityItem : public EntityWorld
	{
	public:
	    EntityItem(UID uidOfEntity, IEntityBase *base); //uidOfEntity is the UID of the reference, not the referenced base!!!
	    EntityItem(const EntityItem &e) = delete; //don't copy me!
        ~EntityItem();

        virtual EntityType getEntityType() { return ENTITYTYPE_WORLD | ENTITYTYPE_ITEM;}

        virtual void loadFromRecord(RecordAccessor rec);

        uint32_t getCount() const;
        void setCount(uint32_t count);


	private:

        uint32_t mCount;
	};

}


#endif /* INCLUDE_IENTITY_H_ */
