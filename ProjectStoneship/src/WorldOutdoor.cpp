/*
 * WorldOutdoor.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: zalasus
 */

#include "WorldOutdoor.h"

#include "RecordAccessor.h"
#include "RecordBuilder.h"
#include "Logger.h"

namespace Stoneship
{


    WorldOutdoor::WorldOutdoor(UID uid)
    : IWorld(uid),
      mWorldName("Unnamed", Record::SUBTYPE_DISPLAY_NAME, this)
    {
    }

    WorldOutdoor::~WorldOutdoor()
    {
    }
    
    void WorldOutdoor::attachNodes(osg::Group *group, ResourceManager *resMan)
    {
        
    }
    
    void WorldOutdoor::detachNodes(osg::Group *group)
    {
        
    }

    String WorldOutdoor::getWorldName() const
    {
        return mWorldName.get();
    }

    void WorldOutdoor::setWorldName(const String &s)
    {
        mWorldName.set(s);
    }

    const std::vector<IEntity*> &WorldOutdoor::getLoadedEntities()
    {
        static std::vector<IEntity*> empty; //just for testing

        return empty;
    }

    uint32_t WorldOutdoor::getLoadedEntityCount() const
    {
        return 0; //testing...
    }
    
    void WorldOutdoor::addEntity(IEntity *e)
    {
        Logger::debug("Added entity " + e->getUID().toString() + " to world " + getUID().toString()); // use Stringify?
    }

    void WorldOutdoor::removeEntity(IEntity *entity)
    {

    }

    void WorldOutdoor::removeEntity(UID uid)
    {

    }

    void WorldOutdoor::loadFromRecord(RecordAccessor &rec, GameCache *gameCache)
    {
        IWorld::loadFromRecord(rec, gameCache);
    }

    void WorldOutdoor::loadAttachment(RecordAccessor &attachment, GameCache *gameCache)
    {
        if(attachment.getHeader().type == Record::TYPE_GROUP && attachment.getHeader().recordCount > 0) //TODO: should it be an error if not followed by GROUP? Maybe check ATTACH flag
        {
            // chunk list is not empty
        }
    }

    std::vector<Chunk*> &WorldOutdoor::getLoadedChunks()
    {
        return mLoadedChunks;
    }
    
    void WorldOutdoor::storeToRecord(RecordBuilder &record)
    {
        IWorld::storeToRecord(record);

        record.beginSubrecord(Record::SUBTYPE_DATA)
            << uint32_t(0)    // flags
            << uint32_t(0);   // world radius
        record.endSubrecord();
    }
    
    void WorldOutdoor::storeAttachment(RecordBuilder &surrounding)
    {
        RecordBuilder chunkGroupBuilder = surrounding.createChildBuilder();
        chunkGroupBuilder.beginGroupRecord(Record::TYPE_GROUP, 0);
        
            
        
        chunkGroupBuilder.endRecord();
    }

}


