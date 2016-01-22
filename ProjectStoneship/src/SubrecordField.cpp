/*
 * ModTracked.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: zalasus
 */

#include "SubrecordField.h"

namespace Stoneship
{

    bool SubrecordFieldTracker::isDirty() const
    {
        for(uint32_t i = 0; i < mTrackedVect.size(); ++i)
        {
            if(mTrackedVect[i]->isDirty())
            {
                return true;
            }
        }

        return false;
    }

    void SubrecordFieldTracker::clean()
    {
        for(uint32_t i = 0; i < mTrackedVect.size(); ++i)
        {
            mTrackedVect[i]->setDirty(false);
        }
    }

    void SubrecordFieldTracker::_track(SubrecordFieldS *tracked)
    {
        mTrackedVect.push_back(tracked);
    }




    SubrecordFieldS::SubrecordFieldS(SubrecordFieldTracker *tracker)
    : mDirty(false)
    {
        if(tracker != nullptr)
        {
            tracker->_track(this);
        }
    }

    SubrecordFieldS::~SubrecordFieldS()
    {
    }

    bool SubrecordFieldS::isDirty() const
    {
        return mDirty;
    }

    void SubrecordFieldS::setDirty(bool dirty)
    {
        mDirty = dirty;
    }




    template <typename T, Record::Subtype RT>
    SubrecordField<T, RT>::SubrecordField(SubrecordFieldTracker *tracker)
    : SubrecordFieldS(tracker),
      mV()
    {
    }

    template <typename T, Record::Subtype RT>
    SubrecordField<T, RT>::SubrecordField(const T &v, SubrecordFieldTracker *tracker)
    : SubrecordFieldS(tracker),
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
    }

    template <typename T, Record::Subtype RT>
    Record::Subtype SubrecordField<T, RT>::getSubtype() const
    {
        return RT;
    }

    template <typename T, Record::Subtype RT>
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
    SubrecordField<T, RT>::operator T()
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
    }
}
