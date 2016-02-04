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

namespace Stoneship
{

    RecordReflector::~RecordReflector()
    {
    }

    void RecordReflector::loadFromRecord(RecordAccessor &record)
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
    }

    void RecordReflector::loadFromModifyRecord(RecordAccessor &record)
    {
        const std::vector<SubrecordFieldS*> &fields = getReflectedFields();

        for(uint32_t i = 0; i < fields.size(); ++i)
        {
            SubrecordFieldS *field = fields[i];

            if(record.getSubrecordCountForType(field->getSubtype()) > 0)
            {
                MGFDataReader reader = record.getReaderForSubrecord(field->getSubtype());

                field->read(reader);
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

            MGFDataWriter writer = b.beginSubrecord(field->getSubtype());

            field->write(writer);

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
                MGFDataWriter writer = b.beginSubrecord(field->getSubtype());

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

    void RecordReflector::_registerForReflection(SubrecordFieldS *field)
    {
        mReflectedVect.push_back(field);
    }

}


