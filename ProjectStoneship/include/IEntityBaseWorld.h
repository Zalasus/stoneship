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

		virtual bool canInteract() const = 0;
		virtual bool onInteract(IEntity *entity, IActor *actor) = 0;

	protected:

		IEntityBaseWorld(UID uid);

		virtual void loadFromRecord(RecordAccessor &record);
		virtual void modifyFromRecord(RecordAccessor &record, Record::ModifyType modType);
		virtual void storeToRecord(RecordBuilder &record);

	private:

		String mModelName;
		float mModelScale;

	};

}


#endif /* INCLUDE_IENTITYBASEWORLD_H_ */
