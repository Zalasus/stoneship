/*
 * WorldOutdoor.h
 *
 *  Created on: Dec 4, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_WORLDOUTDOOR_H_
#define INCLUDE_WORLDOUTDOOR_H_

#include "IWorld.h"

namespace Stoneship
{

    class WorldOutdoor : public IWorld
    {
    public:

        WorldOutdoor(UID uid);
        ~WorldOutdoor();

        // override IWorld
        String getWorldName() const;
        const std::vector<IEntity*> &getLoadedEntities();
        uint32_t getLoadedEntityCount() const;
        void removeEntity(IEntity *entity);
        void removeEntity(UID uid);

        // override IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);


    private:

        String mWorldName;
    };

}


#endif /* INCLUDE_WORLDOUTDOOR_H_ */
