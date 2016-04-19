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
#include "SubrecordField.h"

namespace Stoneship
{
    class IEntity;

    class WorldDungeon : public IWorld
    {
    public:

        WorldDungeon(UID uid);
        ~WorldDungeon();

        // override RecordReflector
        virtual Record::Type getRecordType() const { return Record::TYPE_DUNGEON; };

        // override IWorld
        virtual String getWorldName() const;
        virtual const std::vector<IEntity*> &getLoadedEntities();
        virtual uint32_t getLoadedEntityCount() const;
        virtual void addEntity(IEntity *entity);
        virtual void removeEntity(IEntity *entity);
        virtual void removeEntity(UID uid);

        // override RecordReflector
        virtual void loadFromRecord(RecordAccessor &record);
        virtual void storeToRecord(RecordBuilder &record);
        virtual void postLoad(RecordAccessor &last, RecordAccessor &surrounding);
        virtual void postStore(RecordBuilder &last, RecordBuilder &surrounding);

        void setWorldName(const String &s);

    private:

        SubrecordField<String> mDungeonName;
        std::vector<IEntity*> mEntities;

    };

}


#endif /* INCLUDE_WORLDDUNGEON_H_ */
