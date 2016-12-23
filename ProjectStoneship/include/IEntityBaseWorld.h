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
    class EventPipeline;

	class IEntityBaseWorld : public IEntityBase
	{
	public:

        IEntityBaseWorld(UID uid);
		virtual ~IEntityBaseWorld();

		// override IEntityBase
		virtual IEntity *createEntity(UID uid);

		// interface
		virtual bool canInteract() const = 0;
		virtual bool onInteract(IEntity *entity, IActor *actorr, EventPipeline *pipeline) = 0;

		String getModelName() const;
		void setModelName(const String &s);


	private:

		SubrecordField<String> mModelName;

	};

}


#endif /* INCLUDE_IENTITYBASEWORLD_H_ */
