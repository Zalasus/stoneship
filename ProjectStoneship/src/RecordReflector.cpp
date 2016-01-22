/*
 * IRecordReflector.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: zalasus
 */

#include "RecordReflector.h"

namespace Stoneship
{

    RecordReflector::~RecordReflector()
    {
    }

    void RecordReflector::loadFromRecord(RecordAccessor &record)
    {
        const std::vector<SubrecordFieldS*> &fields = getTrackedFields();

        for(uint32_t i = 0; i < fields.size(); ++i)
        {
            SubrecordFieldS *field = fields[i];

            MGFDataReader reader = record.getReaderForSubrecord(field->getSubtype());

            field->read(reader);
        }
    }

    void RecordReflector::loadFromModifyRecord(RecordAccessor &record)
    {
        const std::vector<SubrecordFieldS*> &fields = getTrackedFields();

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
        const std::vector<SubrecordFieldS*> &fields = getTrackedFields();

        for(uint32_t i = 0; i < fields.size(); ++i)
        {
            SubrecordFieldS *field = fields[i];

            MGFDataWriter writer = b.beginSubrecord(field->getSubtype());

            field->write(writer);

            b.endSubrecord();
        }
    }

    void RecordReflector::storeToModifyRecord(RecordBuilder &b)
    {
        const std::vector<SubrecordFieldS*> &fields = getTrackedFields();

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

}


