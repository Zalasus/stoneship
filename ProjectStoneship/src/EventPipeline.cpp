/*
 * EventPipeline.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: zalasus
 */

#include "EventPipeline.h"

namespace Stoneship
{

    Event::Event(EventType type, Entity *entity, Actor *actor)
    : mType(type),
      mEntity(entity),
      mActor(actor),
      mCanceled(false)
    {
    }

    Event::EventType Event::getType() const
    {
        return mType;
    }

    Entity *Event::getEntity() const
    {
        return mEntity;
    }

    Actor *Event::getActor() const
    {
        return mActor;
    }

    void Event::setCanceled(bool canceled)
    {
        mCanceled = canceled;
    }

    bool Event::isCanceled() const
    {
        return mCanceled;
    }



    EventListener::~EventListener()
    {
    }



    void EventPipeline::addListener(EventListener *listener)
    {
        mListeners.push_back(listener);
    }

    bool EventPipeline::dispatch(const Event &evt)
    {
        Event e = evt;

        for(uint32_t i = 0; i < mListeners.size(); ++i)
        {
            mListeners[i]->onEvent(&e);

            if(e.isCanceled())
            {
                return false;
            }
        }

        return true;
    }

}

