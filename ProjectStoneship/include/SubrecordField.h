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
        virtual SubrecordHeader::SizeType getPredictedDataSize() const;

        bool isDirty() const;
        void setDirty(bool dirty);


    protected:

        bool mDirty;
    };

    template <typename T>
    class SubrecordField : public SubrecordFieldS
    {
    public:

        SubrecordField(const T &v, Record::Subtype subtype, RecordReflector *reflector);

        // override SubrecordFieldS
        virtual void write(MGFDataWriter &writer);
        virtual void read(MGFDataReader &reader);
        virtual Record::Subtype getSubtype() const;
        virtual SubrecordHeader::SizeType getPredictedDataSize() const;

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
        Record::Subtype mSubtype;
    };




    template <typename T>
    SubrecordField<T>::SubrecordField(const T &v, Record::Subtype subtype, RecordReflector *reflector)
    : SubrecordFieldS(reflector),
      mV(v),
      mSubtype(subtype)
    {
    }

    template <typename T>
    const T &SubrecordField<T>::get() const
    {
        return mV;
    }

    template <typename T>
    void SubrecordField<T>::set(const T &v)
    {
        mV = v;
        mDirty = true;
    }

    template <typename T>
    void SubrecordField<T>::setClean(const T &v)
    {
        mV = v;
        mDirty = false;
    }

    template <typename T>
    void SubrecordField<T>::read(MGFDataReader &reader)
    {
        reader >> mV >> MGFDataReader::endr;
    }

    template <typename T>
    void SubrecordField<T>::write(MGFDataWriter &writer)
    {
        writer << mV;
    }

    template <typename T>
    Record::Subtype SubrecordField<T>::getSubtype() const
    {
        return mSubtype;
    }

    /*template <>
    SubrecordHeader::SizeType SubrecordField<String>::getPredictedDataSize() const
    {
        return mV.size() + sizeof(MGFDataWriter::StringSizeType);
    }*/

    template <typename T>
    SubrecordHeader::SizeType SubrecordField<T>::getPredictedDataSize() const
    {
        return sizeof(mV);
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
