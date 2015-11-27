/*
 * WorldObjectBase.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_ENTITYWORLD_H_
#define INCLUDE_ENTITYWORLD_H_

#include "Entity.h"
#include "Types.h"
#include "String.h"
#include "Record.h"

namespace Stoneship
{

    class Actor;

	class EntityWorld : public EntityBase
	{
	public:

		virtual ~EntityWorld();

		String getModelName() const;
		float getModelScale() const;

		virtual bool onInteract(Actor *actor) = 0;

	protected:

		EntityWorld(UID uid, Entity *parent = nullptr, World *world = nullptr);

		void loadFromRecord(RecordAccessor record);

	private:

		World *mWorld;

		String mModelName;
		float mModelScale;

	};

}


#endif /* INCLUDE_ENTITYWORLD_H_ */
