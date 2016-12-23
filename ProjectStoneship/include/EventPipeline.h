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

    class IEntity;
    class IActor;

    class Event
    {
    public:

        enum EventType
        {
            TYPE_USE,
            TYPE_PICKUP
        };

        Event(EventType type, IEntity *entity, IActor *actor);

        EventType getType() const;
        IEntity *getEntity() const;
        IActor *getActor() const;

        void setCanceled(bool canceled);
        bool isCanceled() const;


    private:

        EventType mType;
        IEntity *mEntity;
        IActor *mActor;

        bool mCanceled;
    };

    class IEventListener
    {
    public:

        virtual ~IEventListener();

        virtual void onEvent(Event *event) = 0;

    };
    
    class LambdaListener : public IEventListener
    {
    public:
    
        void setCallback();
    
    };
    
    class EventPipeline
    {
    public:

        void addListener(IEventListener *listener);

        /**
         * @returns true if Event was dispatched to all handlers. false means Event was canceled somewhere
         */
        bool dispatch(const Event &evt);


    private:

        std::vector<IEventListener*> mListeners;

    };

}


#endif /* INCLUDE_EVENTPIPELINE_H_ */
