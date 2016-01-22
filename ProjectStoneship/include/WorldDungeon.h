/*
 * WorldDungeon.h
 *
 *  Created on: Dec 3, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_WORLDDUNGEON_H_
#define INCLUDE_WORLDDUNGEON_H_

#include <vector>

#include "IWorld.h"
#include "Record.h"
#include "Types.h"
#include "String.h"

namespace Stoneship
{
    class IEntity;

    class WorldDungeon : public IWorld
    {
    public:

        WorldDungeon(UID uid);
        ~WorldDungeon();

        // override IWorld
        virtual String getWorldName() const;
        virtual const std::vector<IEntity*> &getLoadedEntities();
        virtual uint32_t getLoadedEntityCount() const;
        virtual void removeEntity(IEntity *entity);
        virtual void removeEntity(UID uid);

        // override IRecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void loadFromModifyRecord(RecordAccessor &record, Record::ModifyType modType);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void storeToModifyRecord(RecordBuilder &record);


    private:

        String mDungeonName;
        std::vector<IEntity*> mEntities;

    };

}


#endif /* INCLUDE_WORLDDUNGEON_H_ */
