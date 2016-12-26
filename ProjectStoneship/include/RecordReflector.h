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

    class GameCache;
    class SubrecordFieldS;
    class RecordAccessor;
    class RecordBuilder;

    class RecordReflector
    {
    public:

        friend class SubrecordFieldS;

        virtual ~RecordReflector();

        // interface TODO: most of these could be pure virtual
        /**
         * Returns the UID this Reflector was created with. In most cases, implementing classes will have a getUID()
         * method and will just redirect getCreatedUID() to that method.
         */
        virtual UID getCreatedUID() const = 0;
        virtual Record::Type getRecordType() const = 0;
        virtual void loadFromRecord(RecordAccessor &record, GameCache *gameCache); // Loads everything. Throws if subrecord is missing
        virtual void loadFromModifyRecord(RecordAccessor &record, GameCache *gameCache); // Just loads fields that have a subrecord
        virtual void storeToRecord(RecordBuilder &b); // Stores everything
        virtual void storeToModifyRecord(RecordBuilder &b); // Just stores dirty fields
        virtual bool mustStore(SubrecordFieldS *field); // true for all fields by default
        virtual bool mustLoad(SubrecordFieldS *field); // true for all fields by default
        //virtual void postStore(RecordBuilder &last, RecordBuilder &surrounding); // called after storeToRecord returns and footers are written
        //virtual void postLoad(RecordAccessor &last, RecordAccessor &surrounding, GameCache *gameCache);
        /**
         * @brief Called on reflectors that had a ATTACH flag set in their headers.
         *
         * If a reflector is loaded from a record that has a set ATTACH flag in it's header,
         * after loadFromRecord() returned, an Accessor for the following record (which should usually be a GROUP)
         * is created and passed to this method. This method is a replacement for the rather
         * generic postLoad() method, which is called on all Reflectors but used by just a few children.
         * Does nothing by default.
         */
        virtual void loadAttachment(RecordAccessor &accessor, GameCache *gameCache);
        virtual void storeAttachment(RecordBuilder &surrounding);

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
