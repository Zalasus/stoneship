#ifndef CHUNK_H
#define CHUNK_H

#include "PosRot.h"
#include "RecordReflector.h"
#include "SubrecordField.h"
#include "RecordAccessor.h"

#include <osg/Vec2i>

namespace Stoneship
{
    class IWorld;
    class IEntity;
    
    class Chunk : public RecordReflector
    {
    public:
        
        static const uint8_t FLAG_END_CHUNK = 1;
        
        Chunk(UID uid, IWorld *parentWorld);
        virtual ~Chunk();

        // implement RecordReflector
        virtual UID getCreatedUID() const;
        virtual Record::Type getRecordType() const;
        virtual void loadFromRecord(RecordAccessor &record, GameCache *gameCache);
        virtual void postLoad(RecordAccessor &last, RecordAccessor &surrounding, GameCache *gameCache);
        
        
        UID getUID() const;
        void setGridPosition(const osg::Vec2i &v);
        osg::Vec2i getGridPosition() const;
        
    protected:

    private:
        
        UID mUID;
        IWorld *mParentWorld;
        SubrecordField<String> mDisplayName;
        SubrecordField<osg::Vec2i> mGridPosition;
        std::vector<IEntity*> mEntities;
        uint8_t mChunkFlags;
        
    };
    
}

#endif // CHUNK_H
