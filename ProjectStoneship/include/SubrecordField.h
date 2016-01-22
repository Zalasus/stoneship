/*
 * ModTracked.h
 *
 *  Created on: Jan 21, 2016
 *      Author: zalasus
 */

#ifndef SRC_MODTRACKED_H_
#define SRC_MODTRACKED_H_

#include <vector>

namespace Stoneship
{

    class SubrecordFieldS;

    class SubrecordFieldTracker
    {
    public:

        friend class SubrecordFieldS;

        bool isDirty() const;
        void clean();
        const std::vector<SubrecordFieldS*> &getTrackedFields();


    private:

        void _track(SubrecordFieldS *tracked);

        std::vector<SubrecordFieldS*> mTrackedVect;

    };

    class SubrecordFieldS
    {
    public:

        SubrecordFieldS(SubrecordFieldTracker *tracker);
        virtual ~SubrecordFieldS();

        // interface
        virtual void write(MGFDataWriter &writer) = 0;
        virtual void read(MGFDataReader &read) = 0;
        virtual Record::Subtype getSubtype() const = 0;

        bool isDirty() const;
        void setDirty(bool dirty);


    protected:

        bool mDirty;
    };

    template <typename T, Record::Subtype RT>
    class SubrecordField : public SubrecordFieldS
    {
    public:

        SubrecordField(SubrecordFieldTracker *tracker);
        SubrecordField(const T &v, SubrecordFieldTracker *tracker);

        // override ModTrackedS
        void write(MGFDataWriter &writer);
        void read(MGFDataReader &reader);
        virtual Record::Subtype getSubtype() const;

        const T &get() const;
        void set(const T &v);
        void setClean(const T &v);

        SubrecordField<T, RT> &operator=(SubrecordField<T, RT> &f);
        SubrecordField<T, RT> &operator=(T &v);
        operator T();
        operator T&();
        operator const T&();


    private:

        T mV;
    };

}


#endif /* SRC_MODTRACKED_H_ */
