/*
 * SimpleArray.h
 *
 *  Created on: 6 Feb 2016
 *      Author: zalasus
 */

#ifndef INCLUDE_SIMPLEARRAY_H_
#define INCLUDE_SIMPLEARRAY_H_

#include "Exception.h"

namespace Stoneship
{

    template <typename T>
    class SimpleArray
    {
    public:

        SimpleArray();
        SimpleArray(uint32_t size);
        ~SimpleArray();

        bool allocate(uint32_t size);
        uint32_t size() const;
        T *ptr() const;

        T &operator [](uint32_t i);
        operator T*() const;
        operator const T*() const;


    private:

        T *mData;
        uint32_t mSize;

    };

    template <typename T>
    SimpleArray<T>::SimpleArray()
    : mData(nullptr),
      mSize(0)
    {
    }

    template <typename T>
    SimpleArray<T>::SimpleArray(uint32_t size)
    : mData(nullptr),
      mSize(size)
    {
        allocate(size);
    }

    template <typename T>
    SimpleArray<T>::~SimpleArray()
    {
        if(mData != nullptr)
        {
            delete[] mData;
            mData = nullptr;
            mSize = 0;
        }
    }

    template <typename T>
    bool SimpleArray<T>::allocate(uint32_t size)
    {
        if(mData != nullptr)
        {
            return false;
        }

        mSize = size;

        if(mSize > 0)
        {
            mData = new T[mSize];
        }

        return true;
    }

    template <typename T>
    uint32_t SimpleArray<T>::size() const
    {
        return mSize;
    }

    template <typename T>
    T *SimpleArray<T>::ptr() const
    {
        return mData;
    }

    template <typename T>
    T &SimpleArray<T>::operator [](uint32_t i)
    {
        if(i >= mSize)
        {
            STONESHIP_EXCEPT(StoneshipException::OUT_OF_BOUNDS, String("Requested index ") + i + " where size was " + mSize);
        }

        return mData[i];
    }

    template <typename T>
    SimpleArray<T>::operator T*() const
    {
        return mData;
    }

    template <typename T>
    SimpleArray<T>::operator const T*() const
    {
        return mData;
    }

}


#endif /* INCLUDE_SIMPLEARRAY_H_ */
