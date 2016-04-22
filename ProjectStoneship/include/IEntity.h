/*
 * Entity.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_IENTITY_H_
#define INCLUDE_IENTITY_H_

#include "RecordReflector.h"
#include "SubrecordField.h"
#include "Types.h"
#include "Record.h"
#include "Inventory.h"

namespace Stoneship
{

    class IEntityBase;
    class IEntityBaseWorld;
    class IEntityBaseItem;
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

		// overrides RecordReflector
		virtual UID getCreatedUID() const;
		virtual Record::Type getRecordType() const;
		virtual void storeToRecord(RecordBuilder &record);
		  // don't need to implement loadFromRecord. The only subrecord in this class is loaded by implementation of IWorld

		// interface
        virtual EntityType getEntityType() const = 0;
        virtual IWorld *getWorld() const = 0;
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

		EntityWorld(UID uidOfEntity, IEntityBaseWorld *base); //uidOfEntity is the UID of the reference, not the referenced base!!!
		EntityWorld(const EntityWorld &e) = delete; //don't copy me!
		~EntityWorld();

        // overrides IEntity
		virtual EntityType getEntityType() const { return ENTITYTYPE_WORLD; }
		virtual IWorld *getWorld() const;
		void spawn(IWorld *w);
        void despawn();

		float getScale() const;
		void setScale(float f);
        void remove();


	private:

		IWorld *mWorld;
		//SubrecordField<PosRot> mPositionRotation;
		SubrecordField<float> mScale;
	};

	class EntityItem : public EntityWorld
	{
	public:
	    EntityItem(UID uidOfEntity, IEntityBaseItem *base); //uidOfEntity is the UID of the reference, not the referenced base!!!
	    EntityItem(const EntityItem &e) = delete; //don't copy me!
        ~EntityItem();

        // overrides EntityWorld
        virtual EntityType getEntityType() { return ENTITYTYPE_WORLD | ENTITYTYPE_ITEM; }

        uint32_t getCount() const;
        void setCount(uint32_t count);


	private:

        SubrecordField<uint32_t> mCount;
	};

}


#endif /* INCLUDE_IENTITY_H_ */
