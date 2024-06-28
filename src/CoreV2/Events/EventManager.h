#pragma once

#include "Event.h"
#include <iostream>
#include <list>
#include <map>
#include <vector>

class BaseHandlerFunction {
public:
    virtual bool call(BaseEvent *) { return true; };
};

template<class T, class EventT>
class HandlerFunctionInstance : public BaseHandlerFunction {
public:
    typedef bool (T::*EventHandlerFunction)(EventT *const);

    HandlerFunctionInstance() : handlerFunction(), instance(nullptr) {}
    HandlerFunctionInstance(T *source, EventHandlerFunction handler) {
        instance = source;
        handlerFunction = handler;
    }

    bool call(BaseEvent *event) override {
        return (instance->*handlerFunction)((EventT *) event);
    }

private:
    EventHandlerFunction handlerFunction;
    T *instance;
};

class EventManager {
public:
    EventManager() {
        current = new TEventList();
        next = new TEventList();
    }

    typedef std::list<BaseHandlerFunction *> THandlerFunctionsList;
    typedef std::list<BaseEvent *> TEventList;

    template<class T, class EventT>
    void registerEventReceiver(T *const instance, bool (T::*function)(EventT *const)) {
        if (eventReceivers.find(EventT::TypeId()) == eventReceivers.end()) {
            eventReceivers[EventT::TypeId()] = new THandlerFunctionsList();
        }

        eventReceivers[EventT::TypeId()]->push_back(new HandlerFunctionInstance<T, EventT>(instance, function));
    }

    template<class EventT>
    EventT* createEvent() {
        auto evt = new EventT();
        evt->m_EventManager = this;

        return evt;
    }

    template<typename EventT>
    void triggerEvent(EventT *const event) {
        THandlerFunctionsList *handlers = eventReceivers[event->getTypeId()];

        if (!handlers) {
            return;
        }

        for (auto &handler: *handlers) {
            if (handler) {
                handler->call(event);
            }
        }
    }

    template<typename EventT>
    void queueEvent(EventT *const event) {
        next->push_back(event);
    }

    bool processEvents() {
        std::swap(current, next);

        current->sort(SortEventsByTypeName());

        THandlerFunctionsList *handlers;
        int previousEventType = -1, eventType;

        for (auto const &event: *current) {
            eventType = (int)event->getTypeId();

            if (eventReceivers.find(eventType) == eventReceivers.end()) {
                delete event;
                continue;
            }

            if (previousEventType != eventType) {
                handlers = eventReceivers[eventType];
            }

            for (auto const &handler: *handlers) {

                handler->call(event);
            }

            previousEventType = eventType;

            delete event;
        }

        current->clear();

        return true;
    }

private:
    std::map<unsigned int, THandlerFunctionsList *> eventReceivers;
    TEventList *current;
    TEventList *next;

    struct SortEventsByTypeName {
        inline bool operator()(BaseEvent *lhs, BaseEvent *rhs) {
            return lhs->getTypeId() < rhs->getTypeId();
        }
    };
};