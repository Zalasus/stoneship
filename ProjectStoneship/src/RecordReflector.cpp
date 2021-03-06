/*
 * IRecordReflector.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: zalasus
 */

#include "RecordReflector.h"

#include "SubrecordField.h"
#include "RecordAccessor.h"
#include "RecordBuilder.h"
#include "Exception.h"
#include "GameCache.h"

namespace Stoneship
{

    RecordReflector::~RecordReflector()
    {
    }

    const std::vector<SubrecordFieldS*> &RecordReflector::getReflectedFields()
    {
        return mReflectedVect;
    }

    void RecordReflector::loadFromRecord(RecordAccessor &record, GameCache *gameCache)
    {
        const std::vector<SubrecordFieldS*> &fields = getReflectedFields();
        
        for(uint32_t i = 0; i < fields.size(); ++i)
        {
            SubrecordFieldS *field = fields[i];

            // load only fields reported to be required
            if(!mustLoad(field))
            {
                continue;
            }

            try
            {
                MGFDataReader reader = record.getReaderForSubrecord(field->getSubtype());
                field->read(reader);
                reader.skipToEnd(); // we might have not read every byte in subrecord. skip remaining so next call to getReaderForSubrecord doesn't read garbage header

            }catch(StoneshipException &e)
            {
                if(e.getType() == StoneshipException::SUBRECORD_NOT_FOUND)
                {
                    STONESHIP_EXCEPT(StoneshipException::SUBRECORD_NOT_FOUND, "Error loading " + getCreatedUID().toString() + ". Non-optional subrecord field " + field->getSubtype() + " was not found in record.");

                }else
                {
                    throw;
                }
            }
        }

        //TODO: We could handle reading of this using an optional SubrecordField (once we have implemented proper handling for optional fields)
        if(record.getSubrecordCountForType(Record::SUBTYPE_EDITOR_NAME))
        {
            record.getReaderForSubrecord(Record::SUBTYPE_EDITOR_NAME)
            		>> mEditorName
					>> MGFDataReader::endr;
        }

        if(record.getSubrecordCountForType(Record::SUBTYPE_EDITOR_COMMENT))
        {
            record.getReaderForSubrecord(Record::SUBTYPE_EDITOR_COMMENT)
            		>> mEditorComment
					>> MGFDataReader::endr;
        }
    }

    void RecordReflector::loadFromModifyRecord(RecordAccessor &record, GameCache *gameCache)
    {
        const std::vector<SubrecordFieldS*> &fields = getReflectedFields();

        for(uint32_t i = 0; i < fields.size(); ++i)
        {
            SubrecordFieldS *field = fields[i];

            if(record.getSubrecordCountForType(field->getSubtype()) > 0)
            {
                MGFDataReader reader = record.getReaderForSubrecord(field->getSubtype());
                field->read(reader);
                reader.skipToEnd(); // we might have not read every byte in subrecord. skip remaining so next call to getReaderForSubrecord doesn't read garbage header
            }
        }
    }

    void RecordReflector::storeToRecord(RecordBuilder &b)
    {
        const std::vector<SubrecordFieldS*> &fields = getReflectedFields();

        for(uint32_t i = 0; i < fields.size(); ++i)
        {
            SubrecordFieldS *field = fields[i];

            if(!mustStore(field))
            {
                continue;
            }

            MGFDataWriter writer = b.beginSubrecord(field->getSubtype(), field->getPredictedDataSize());
            field->write(writer);
            b.endSubrecord();
        }

        if(!mEditorName.empty())
        {
            b.beginSubrecord(Record::SUBTYPE_EDITOR_NAME, mEditorName.length() + 5)
            		<< mEditorName;
            b.endSubrecord();
        }

        if(!mEditorComment.empty())
        {
            b.beginSubrecord(Record::SUBTYPE_EDITOR_COMMENT, mEditorComment.length() + 5)
            		<< mEditorComment;
            b.endSubrecord();
        }
    }

    void RecordReflector::storeToModifyRecord(RecordBuilder &b)
    {
        const std::vector<SubrecordFieldS*> &fields = getReflectedFields();

        for(uint32_t i = 0; i < fields.size(); ++i)
        {
            SubrecordFieldS *field = fields[i];

            if(field->isDirty())
            {
                MGFDataWriter writer = b.beginSubrecord(field->getSubtype(), field->getPredictedDataSize());
                field->write(writer);
                b.endSubrecord();
            }
        }
    }

    bool RecordReflector::mustStore(SubrecordFieldS *field)
    {
        return true;
    }

    bool RecordReflector::mustLoad(SubrecordFieldS *field)
    {
        return true;
    }

    /*void RecordReflector::postStore(RecordBuilder &last, RecordBuilder &surrounding)
    {
    }

    void RecordReflector::postLoad(RecordAccessor &last, RecordAccessor &surrounding, GameCache *gameCache)
    {
    }*/

    void RecordReflector::loadAttachment(RecordAccessor &attachment, GameCache *gameCache)
    {
    }

    void RecordReflector::storeAttachment(RecordBuilder &attachmentBuilder)
    {
    }

    bool RecordReflector::isDirty() const
    {
        for(uint32_t i = 0; i < mReflectedVect.size(); ++i)
        {
            if(mReflectedVect[i]->isDirty())
            {
                return true;
            }
        }

        return false;
    }

    void RecordReflector::clean()
    {
        for(uint32_t i = 0; i < mReflectedVect.size(); ++i)
        {
            mReflectedVect[i]->setDirty(false);
        }
    }

    void RecordReflector::setEditorName(const String &s)
    {
        mEditorName = s;
    }

    void RecordReflector::setEditorComment(const String &s)
    {
    	mEditorComment = s;
    }

    String RecordReflector::getEditorName() const
    {
        return mEditorName;
    }

    String RecordReflector::getEditorComment() const
    {
    	return mEditorComment;
    }

    void RecordReflector::_registerForReflection(SubrecordFieldS *field)
    {
        mReflectedVect.push_back(field);
    }

}


