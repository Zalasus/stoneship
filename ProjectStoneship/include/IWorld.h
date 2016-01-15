/*
 * World.h
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_IWORLD_H_
#define INCLUDE_IWORLD_H_

#include "Types.h"
#include "Record.h"
#include "IRecordLoadable.h"
#include "IRecordStoreable.h"
#include "String.h"

namespace Stoneship
{

    class IEntity;

	class IWorld : public IRecordLoadable, public IRecordStoreable
	{
	public:

	    IWorld(UID uid);
	    virtual ~IWorld();

	    UID getUID() const;

	    virtual String getWorldName() const = 0;

	    virtual const std::vector<IEntity*> &getLoadedEntities() = 0;
	    virtual uint32_t getLoadedEntityCount() const = 0;

	    virtual void removeEntity(IEntity *entity) = 0;
	    virtual void removeEntity(UID uid) = 0;


	private:

	    UID mUID;
	};

}



#endif /* INCLUDE_IWORLD_H_ */
