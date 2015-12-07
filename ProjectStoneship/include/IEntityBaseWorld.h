/*
 * WorldObjectBase.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_IENTITYBASEWORLD_H_
#define INCLUDE_IENTITYBASEWORLD_H_

#include "IEntityBase.h"
#include "Types.h"
#include "String.h"
#include "Record.h"

namespace Stoneship
{

    class IEntity;
    class IActor;

	class IEntityBaseWorld : public IEntityBase
	{
	public:

		virtual ~IEntityBaseWorld();

		String getModelName() const;
		float getModelScale() const;

		virtual IEntity *createEntity(UID uid);

		virtual bool onInteract(IEntity *entity, IActor *actor) = 0;

	protected:

		IEntityBaseWorld(UID uid);

		void loadFromRecord(RecordAccessor record);

	private:

		String mModelName;
		float mModelScale;

	};

}


#endif /* INCLUDE_IENTITYBASEWORLD_H_ */