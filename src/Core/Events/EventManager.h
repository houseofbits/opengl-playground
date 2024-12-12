#pragma once

#include "Event.h"
#include "../Reflection/Identity.h"
#include "../Helper/Log.h"
#include "SystemEvent.h"
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>

class EventHandler {
public:
    virtual ~EventHandler() = default;

    virtual Identity::Type getId() {
        return 0;
    }
};

class BaseHandlerFunction {
public:
    virtual ~BaseHandlerFunction() = default;

    virtual void call(BaseEvent *) = 0;

    virtual Identity::Type getId() = 0;

    virtual bool compare(EventHandler *other) {
        return false;
    }
};

template<class T, class EventT>
class HandlerFunctionInstance : public BaseHandlerFunction {
public:
    typedef void (T::*EventHandlerFunction)(const EventT &);

    HandlerFunctionInstance() : handlerFunction(), instance(nullptr) {}

    HandlerFunctionInstance(T *source, EventHandlerFunction handler) {
        instance = source;
        handlerFunction = handler;
    }

    void call(BaseEvent *event) override {
        (instance->*handlerFunction)((const EventT &) *event);
    }

    Identity::Type getId() override {
        return instance->getId();
    }

    bool compare(EventHandler *other) override {
        return other == instance;
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
    void registerEventReceiver(T *const instance, void (T::*function)(const EventT &)) {
        if (eventReceivers.find(EventT::TypeId()) == eventReceivers.end()) {
            eventReceivers[EventT::TypeId()] = new THandlerFunctionsList();
        }

        eventReceivers[EventT::TypeId()]->push_back(new HandlerFunctionInstance<T, EventT>(instance, function));
    }

    template<class EventT, class... Ts>
    EventT &createEvent(Ts... args) {
        auto evt = new EventT(args...);
        evt->m_EventManager = this;

        return *evt;
    }

    template<typename EventT>
    void triggerEvent(EventT &event) {
        THandlerFunctionsList *handlers = eventReceivers[event.getTypeId()];

        if (!handlers) {
            return;
        }

        for (auto &handler: *handlers) {
            if (handler) {
                handler->call(&event);
            }
        }
    }

    template<class EventT, class... Ts>
    void triggerEvent(Ts... args) {
        triggerEvent(createEvent<EventT>(args...));
    }

    template<typename EventT>
    void queueEvent(EventT &event) {
        next->push_back(&event);
    }

    template<class EventT, class... Ts>
    void queueEvent(Ts... args) {
        queueEvent(createEvent<EventT>(args...));
    }

    void processEvents() {
        std::swap(current, next);

        current->sort(SortEventsByTypeName());

        THandlerFunctionsList *handlers;
        int previousEventType = -1, eventType;

        for (auto const &event: *current) {
            eventType = (int) event->getTypeId();

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
    }

    void removeEventHandler(EventHandler *handler) {
        for (const auto &functorList: eventReceivers) {
            auto it = std::find_if(functorList.second->begin(), functorList.second->end(),
                                   [&](BaseHandlerFunction *functor) {
                                       return functor->compare(handler);
                                   });

            if (it != functorList.second->end()) {
                delete (*it);
                functorList.second->erase(it);
            }
        }
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