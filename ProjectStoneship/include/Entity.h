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

	class Entity
	{
	public:

		enum EntityType
		{
			ENTITYTYPE_DEFAULT,
			ENTITYTYPE_WORLD
		};

		Entity(UID uid, EntityBase *base);
		virtual ~Entity();

		virtual EntityType getEntityType() { return ENTITYTYPE_DEFAULT;}

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

		void remove();
		void setHidden(bool hidden);
		bool isHidden() const;

	private:

		WorldManager *mWorldManager;

		bool mHidden;

	};

}


#endif /* INCLUDE_ENTITY_H_ */
