/*
 * World.h
 *
 *  Created on: Nov 20, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_IWORLD_H_
#define INCLUDE_IWORLD_H_

#include <RecordReflector.h>
#include "Types.h"
#include "Record.h"
#include "String.h"

namespace Stoneship
{

    class IEntity;

	class IWorld : public RecordReflector
	{
	public:

	    IWorld(UID uid);
	    virtual ~IWorld();

	    // overrides IRecordReflector
       virtual UID getCreatedUID();

	    // interface
	    virtual String getWorldName() const = 0;
	    virtual const std::vector<IEntity*> &getLoadedEntities() = 0;
	    virtual uint32_t getLoadedEntityCount() const = 0;
	    virtual void removeEntity(IEntity *entity) = 0;
	    virtual void removeEntity(UID uid) = 0;

        UID getUID() const;


	private:

	    UID mUID;
	};

}



#endif /* INCLUDE_IWORLD_H_ */
