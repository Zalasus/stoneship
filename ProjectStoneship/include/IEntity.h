/*
 * Entity.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_IENTITY_H_
#define INCLUDE_IENTITY_H_

#include <RecordReflector.h>
#include "Types.h"
#include "Record.h"
#include "Inventory.h"

namespace Stoneship
{

    class IEntityBase;
    class IWorld;

	class IEntity : public RecordReflector
	{
	public:

        // TODO: create bitmask type for this
		typedef uint32_t EntityType;
		static const EntityType ENTITYTYPE_DEFAULT = 1;
		static const EntityType ENTITYTYPE_WORLD = 2;
		static const EntityType ENTITYTYPE_ITEM = 4;
		static const EntityType ENTITYTYPE_CONTAINER = 8;


		IEntity(UID uid, IEntityBase *base);
		virtual ~IEntity();

		// overrides IRecordReflector
		virtual UID::Ordinal getCreatedBy();

		// interface
        virtual EntityType getEntityType() = 0;
        virtual void spawn(IWorld *w) = 0;
        virtual void despawn() = 0;

        UID getUID() const;
        IEntityBase *getBase() const;

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

        // overrides IEntity
		virtual EntityType getEntityType() { return ENTITYTYPE_WORLD; }
		void spawn(IWorld *w);
        void despawn();

        // overrides IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record, Record::ModifyType modType);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeToModifyRecord(RecordBuilder &record);

		void remove();

	private:

		IWorld *mWorld;
	};

	class EntityContainer : public EntityWorld
	{
	public:
	    EntityContainer(UID uidOfEntity, IEntityBase *base); //uidOfEntity is the UID of the reference, not the referenced base!!!
	    EntityContainer(const EntityWorld &e) = delete; //don't copy me!
        ~EntityContainer();

        // overrides EntityWorld
        virtual EntityType getEntityType() { return ENTITYTYPE_WORLD | ENTITYTYPE_CONTAINER; }

        // overrides IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record, Record::ModifyType modType);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeToModifyRecord(RecordBuilder &record);

        Inventory &getInventory();

	private:

        Inventory mInventory;

	};

	class EntityItem : public EntityWorld
	{
	public:
	    EntityItem(UID uidOfEntity, IEntityBase *base); //uidOfEntity is the UID of the reference, not the referenced base!!!
	    EntityItem(const EntityItem &e) = delete; //don't copy me!
        ~EntityItem();

        // overrides EntityWorld
        virtual EntityType getEntityType() { return ENTITYTYPE_WORLD | ENTITYTYPE_ITEM; }

        // overrides IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record, Record::ModifyType modType);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeToModifyRecord(RecordBuilder &record);

        uint32_t getCount() const;
        void setCount(uint32_t count);


	private:

        uint32_t mCount;
	};

}


#endif /* INCLUDE_IENTITY_H_ */
