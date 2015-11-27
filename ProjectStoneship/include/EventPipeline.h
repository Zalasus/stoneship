/*
 * EventPipeline.h
 *
 *  Created on: Nov 24, 2015
 *      Author: zalasus
 */

#ifndef INCLUDE_EVENTPIPELINE_H_
#define INCLUDE_EVENTPIPELINE_H_

#include <vector>

#include "Types.h"

namespace Stoneship
{

    class Entity;
    class Actor;

    class Event
    {
    public:

        enum EventType
        {
            TYPE_USE,
            TYPE_PICKUP
        };

        Event(EventType type, Entity *entity, Actor *actor);

        EventType getType() const;
        Entity *getEntity() const;
        Actor *getActor() const;

        void setCanceled(bool canceled);
        bool isCanceled() const;


    private:

        EventType mType;
        Entity *mEntity;
        Actor *mActor;

        bool mCanceled;
    };

    class EventListener
    {
    public:

        virtual ~EventListener();

        virtual void onEvent(Event *event) = 0;

    };

    class EventPipeline
    {
    public:

        void addListener(EventListener *listener);

        /**
         * @returns true if Event was dispatched to all handlers. false means Event was canceled somewhere
         */
        bool dispatch(const Event &evt);


    private:

        std::vector<EventListener*> mListeners;

    };

}


#endif /* INCLUDE_EVENTPIPELINE_H_ */
