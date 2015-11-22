/*
 * WorldObjectBase.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_WORLDENTITYBASE_H_
#define INCLUDE_WORLDENTITYBASE_H_

#include "EntityBase.h"
#include "Player.h"

namespace Stoneship
{

	class Entity;

	class WorldEntityBase : public EntityBase
	{
	public:

		virtual ~WorldEntityBase();

		String getModelName() const;
		float getModelScale() const;

		virtual bool onInteract(Entity *entity, Actor *actor) = 0;

	protected:

		WorldEntityBase(UID uid);

		void loadFromRecord(RecordAccessor record);

	private:

		String mModelName;
		float mModelScale;

	};

}


#endif /* INCLUDE_WORLDENTITYBASE_H_ */
