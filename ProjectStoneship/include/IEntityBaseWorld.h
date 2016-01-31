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
#include "SubrecordField.h"

namespace Stoneship
{

    class IEntity;
    class IActor;

	class IEntityBaseWorld : public IEntityBase
	{
	public:

        IEntityBaseWorld(UID uid);
		virtual ~IEntityBaseWorld();

		// override IEntityBase
		virtual IEntity *createEntity(UID uid);

		// interface
		virtual bool canInteract() const = 0;
		virtual bool onInteract(IEntity *entity, IActor *actor) = 0;

		String getModelName() const;
        float getModelScale() const;


	private:

		SubrecordField<String, Record::SUBTYPE_WORLD_MODEL_NAME> mModelName;
		SubrecordField<float, Record::SUBTYPE_SCALE> mModelScale;

	};

}


#endif /* INCLUDE_IENTITYBASEWORLD_H_ */
