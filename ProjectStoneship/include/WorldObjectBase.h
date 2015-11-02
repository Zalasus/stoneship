/*
 * WorldObjectBase.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_WORLDOBJECTBASE_H_
#define INCLUDE_WORLDOBJECTBASE_H_

#include "Entity.h"
#include "Player.h"

namespace Stoneship
{

	class WorldObjectBase : public EntityBase
	{
	public:

		String getModelName() const;

		virtual void load(RecordAccessor record);

		//virtual void onInteract(Player &player) = 0;

	protected:

		WorldObjectBase(Record::Type recordType, UID uid);


	private:

		String mModelName;

	};

}


#endif /* INCLUDE_WORLDOBJECTBASE_H_ */
