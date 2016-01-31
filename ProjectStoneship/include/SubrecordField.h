/*
 * ModTracked.h
 *
 *  Created on: Jan 21, 2016
 *      Author: zalasus
 */

#ifndef SRC_MODTRACKED_H_
#define SRC_MODTRACKED_H_

#include "Record.h"
#include "RecordReflector.h"
#include "MGFDataWriter.h"
#include "MGFDataReader.h"

namespace Stoneship
{

    class SubrecordFieldS
    {
    public:

        SubrecordFieldS(RecordReflector *reflector);
        virtual ~SubrecordFieldS();

        // interface
        virtual void write(MGFDataWriter &writer) = 0;
        virtual void read(MGFDataReader &read) = 0;
        virtual Record::Subtype getSubtype() const = 0;
        virtual bool isOptional() = 0;

        bool isDirty() const;
        void setDirty(bool dirty);


    protected:

        bool mDirty;
    };

    template <typename T, Record::Subtype RT>
    class SubrecordField : public SubrecordFieldS
    {
    public:

        SubrecordField(const T &v, RecordReflector *reflector);

        // override SubrecordFieldS
        virtual void write(MGFDataWriter &writer);
        virtual void read(MGFDataReader &reader);
        Record::Subtype getSubtype() const;

        const T &get() const;
        void set(const T &v);
        void setClean(const T &v);

        /*SubrecordField<T, RT> &operator=(SubrecordField<T, RT> &f);
        SubrecordField<T, RT> &operator=(T &v);
        operator T() const;
        explicit operator T&();
        explicit operator const T&();*/


    private:

        T mV;
    };




    template <typename T, Record::Subtype RT>
    SubrecordField<T, RT>::SubrecordField(const T &v, RecordReflector *reflector)
    : SubrecordFieldS(reflector),
      mV(v)
    {
    }

    template <typename T, Record::Subtype RT>
    const T &SubrecordField<T, RT>::get() const
    {
        return mV;
    }

    template <typename T, Record::Subtype RT>
    void SubrecordField<T, RT>::set(const T &v)
    {
        mV = v;
        mDirty = true;
    }

    template <typename T, Record::Subtype RT>
    void SubrecordField<T, RT>::setClean(const T &v)
    {
        mV = v;
        mDirty = false;
    }

    template <typename T, Record::Subtype RT>
    Record::Subtype SubrecordField<T, RT>::getSubtype() const
    {
        return RT;
    }

    /*template <typename T, Record::Subtype RT>
    SubrecordField<T, RT> &SubrecordField<T, RT>::operator=(SubrecordField<T, RT> &f)
    {
        set(f.get());
    }

    template <typename T, Record::Subtype RT>
    SubrecordField<T, RT> &SubrecordField<T, RT>::operator=(T &v)
    {
        set(v);
    }

    template <typename T, Record::Subtype RT>
    SubrecordField<T, RT>::operator T() const
    {
        return mV;
    }

    template <typename T, Record::Subtype RT>
    SubrecordField<T, RT>::operator T&()
    {
        // this does not neccessarily apply. one could use the reference without writing to it.
        // for now, let's hope the compiler will choose the value-cast or const-ref-cast in those cases
        mDirty = true;

        return mV;
    }

    template <typename T, Record::Subtype RT>
    SubrecordField<T, RT>::operator const T&()
    {
        return mV;
    }*/

}


#endif /* SRC_MODTRACKED_H_ */
