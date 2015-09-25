/*
 * WorldObjectBase.h
 *
 *  Created on: 16.09.2015
 *      Author: Zalasus
 */

#ifndef INCLUDE_WORLDOBJECTBASE_H_
#define INCLUDE_WORLDOBJECTBASE_H_

#include "Entity.h"

namespace Stoneship
{

	class WorldObjectBase : public EntityBase
	{
	public:
		friend class MGFDataReader;  // MGFDataReader must access elements


		String getModelName() const;


	protected:

		WorldObjectBase(Record::Type recordType, UID uid);


	private:

		String mModelName;

	};

}


#endif /* INCLUDE_WORLDOBJECTBASE_H_ */
