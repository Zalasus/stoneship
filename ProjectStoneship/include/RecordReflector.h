/*
 * IRecordReflector.h
 *
 *  Created on: Jan 20, 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_RECORDREFLECTOR_H_
#define INCLUDE_RECORDREFLECTOR_H_

#include <vector>

#include "Record.h"

namespace Stoneship
{

    class SubrecordFieldS;
    class RecordAccessor;
    class RecordBuilder;

    class RecordReflector
    {
    public:

        friend class SubrecordFieldS;

        virtual ~RecordReflector();

        // interface
        /**
         * Returns the UID this Reflector was created with. In most cases, implementing classes will have a getUID()
         * method and will just redirect getCreatedUID() to that method.
         */
        virtual UID getCreatedUID() const = 0;
        virtual Record::Type getRecordType() const = 0;
        virtual void loadFromRecord(RecordAccessor &record); // Loads everything. Throws if subrecord is missing
        virtual void loadFromModifyRecord(RecordAccessor &record); // Just loads fields that have a subrecord
        virtual void storeToRecord(RecordBuilder &b); // Stores everything
        virtual void storeToModifyRecord(RecordBuilder &b); // Just stores dirty fields
        virtual bool mustStore(SubrecordFieldS *field); // true for all fields by default
        virtual bool mustLoad(SubrecordFieldS *field); // true for all fields by default
        virtual void postStore(RecordBuilder &last, RecordBuilder &surrounding); // called after storeToRecord returns and footers are written
        virtual void postLoad(RecordAccessor &last, RecordAccessor &surrounding);

        bool isDirty() const;
        /**
         * Resets the dirty state of all fields. Note that this is not neccessarily a good idea.
         * ATM there is no case in which the dirty state would have to change to anything but true.
         */
        void clean();
        const std::vector<SubrecordFieldS*> &getReflectedFields();

        // does this really belong here?
        void setEditorName(const String &s);
        String getEditorName() const;

    private:

        void _registerForReflection(SubrecordFieldS *field);

        std::vector<SubrecordFieldS*> mReflectedVect;

        String mEditorName;
    };

}


#endif /* INCLUDE_RECORDREFLECTOR_H_ */
