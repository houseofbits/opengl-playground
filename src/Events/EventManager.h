#pragma once

#include "Event.h"
#include <map>
#include <vector>
#include <list>

class BaseHandlerFunction
{
public:
    virtual bool call(BaseEvent *) { return true; };
};

template <class T, class EventT>
class HandlerFunctionInstance : public BaseHandlerFunction
{
public:
    typedef bool (T::*EventHandlerFunction)(EventT *const);

    HandlerFunctionInstance() {}
    HandlerFunctionInstance(T *source, EventHandlerFunction handler)
    {
        instance = source;
        handlerFunction = handler;
    }

    bool call(BaseEvent *event)
    {
        return (instance->*handlerFunction)((EventT *)event);
    }

private:
    EventHandlerFunction handlerFunction;
    T *instance;
};

class EventManager
{
public:
    EventManager()
    {
        current = new TEventList();
        next = new TEventList();
    }

    typedef std::list<BaseHandlerFunction *> THandlerFunctionsList;
    typedef std::list<BaseEvent *> TEventList;

    template <class T, class EventT>
    void registerEventReceiver(T *const instance, bool (T::*function)(EventT *const))
    {
        THandlerFunctionsList *handlers = eventReceivers[EventT::TypeId()];

        if (handlers == nullptr)
        {
            handlers = new THandlerFunctionsList();
            eventReceivers[EventT::TypeId()] = handlers;
        }

        handlers->push_back(new HandlerFunctionInstance<T, EventT>(instance, function));
    }

    template <typename EventT>
    void triggerEvent(EventT *const event)
    {
        THandlerFunctionsList *handlers = eventReceivers[event->getTypeId()];

        if (!handlers)
        {
            return;
        }

        for (auto &handler : *handlers)
        {
            if (handler)
            {
                handler->call(event);
            }
        }
    }

    template <typename EventT>
    void queueEvent(EventT *const event)
    {
        next->push_back(event);
    }

    bool processEvents()
    {
        std::swap(current, next);

        current->sort(SortEventsByTypeName());

        THandlerFunctionsList *handlers;
        int previousEventType = -1, eventType;

        for (auto const &event : *current)
        {
            eventType = event->getTypeId();

            if (previousEventType != eventType)
            {
                handlers = eventReceivers[eventType];
            }

            for (auto const &handler : *handlers)
            {
                handler->call(event);
            }

            previousEventType = eventType;
        }

        // while (!current->empty())
        // {
        //     delete current->front();
        //     current->pop_front();
        // }

        current->clear();

        return true;
    }

private:
    std::map<unsigned int, THandlerFunctionsList *> eventReceivers;
    TEventList *current;
    TEventList *next;

    struct SortEventsByTypeName
    {
        inline bool operator()(BaseEvent *lhs, BaseEvent *rhs)
        {
            return lhs->getTypeId() < rhs->getTypeId();
        }
    };
};