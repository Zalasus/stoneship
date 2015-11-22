/*
 * Entity.h
 *
 *  Created on: 15.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include "Record.h"

namespace Stoneship
{

	class WorldManager;
	class WorldEntityBase;

	class Entity
	{


	};

	class WorldEntity : public Entity
	{
	public:
		WorldEntity(UID uidOfEntity, WorldEntityBase *base, WorldManager *manager); //uidOfEntity is the UID of the reference, not the referenced base!!!
		WorldEntity(const WorldEntity &e) = delete; //don't copy me!
		~WorldEntity();

		UID getUID() const;
		WorldEntityBase *getBase() const;

		void remove();
		void setHidden(bool hidden);
		bool isHidden() const;

	private:

		UID mUID;
		WorldEntityBase *mBase;
		WorldManager *mWorldManager;

		bool mHidden;

	};

}


#endif /* INCLUDE_ENTITY_H_ */
