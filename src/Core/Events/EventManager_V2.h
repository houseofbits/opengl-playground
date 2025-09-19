#pragma once

#ifndef EVENTMANAGER_V1

#include "Event.h"
#include "../Reflection/Identity.h"
#include "../Helper/Log.h"
#include <list>
#include <map>
#include <queue>
#include <atomic>

class EventListener;
class EventManager;

class RefCountEvent {
public:
    explicit RefCountEvent(BaseEvent *event) : m_event(event), m_refCount(0) {
    }

    void addReference() { ++m_refCount; }

    void removeReference() {
        --m_refCount;
        invalidate();
    }

    bool isValid() {
        invalidate();

        return m_refCount > 0;
    }

    [[nodiscard]] BaseEvent *getPtr() const {
        return m_event;
    }

    void invalidate() {
        if (m_refCount <= 0 && m_event != nullptr) {
            m_refCount = 0;
            delete m_event;
            m_event = nullptr;
        }
    }

private:
    BaseEvent *m_event;
    std::atomic<int> m_refCount;
};

class BaseHandlerFunction {
public:
    virtual ~BaseHandlerFunction() = default;

    virtual void call(EventListener *inst, BaseEvent *event) = 0;
};

template<class T, class EventT>
class HandlerFunctionInstance final : public BaseHandlerFunction {
public:
    typedef void (T::*EventHandlerFunction)(const EventT &);

    explicit HandlerFunctionInstance(EventHandlerFunction handler) {
        handlerFunction = handler;
    }

    void call(EventListener *inst, BaseEvent *event) override {
        (static_cast<T *>(inst)->*handlerFunction)(static_cast<const EventT &>(*event));
    }

private:
    EventHandlerFunction handlerFunction;
};

class EventListener {
public:
    virtual ~EventListener() = default;

    void processEvents() {
        while (!m_events.empty()) {
            if (const auto event = m_events.front(); event->isValid()) {
                handleEvent(event->getPtr());
                event->removeReference();
            }
            m_events.pop();
        }
    }

    virtual Identity::Type getId() {
        return 0;
    }

private:
    template<class T, class EventT>
    void registerEventHandlerFunction(void (T::*function)(const EventT &)) {
        m_eventHandlersFunctions[EventT::TypeId()] = new HandlerFunctionInstance<T, EventT>(function);
    }

    void handleEvent(BaseEvent *event) {
        if (const auto handler = m_eventHandlersFunctions.find(event->getTypeId());
            handler != m_eventHandlersFunctions.end()) {
            handler->second->call(this, event);
        }
    }

    void registerEvent(RefCountEvent &event) {
        event.addReference();
        m_events.push(&event);
    }

    std::map<BaseEvent::IdType, BaseHandlerFunction *> m_eventHandlersFunctions;
    std::queue<RefCountEvent *> m_events;

    friend class EventManager;
};

class EventManager {
public:
    typedef std::list<EventListener *> EventListenersList;

    template<class T, class EventT>
    void registerEventReceiver(T *const instance, void (T::*function)(const EventT &)) {
        m_eventListeners[EventT::TypeId()].push_back(instance);
        instance->registerEventHandlerFunction(function);
    }

    template<class T, class EventT>
    void registerEventHandlerFunction(T *const instance, void (T::*function)(const EventT &)) {
        m_eventListeners[EventT::TypeId()].push_back(instance);
        instance->registerEventHandlerFunction(function);
    }

    template<class EventT, class... Ts>
    void queueEvent(Ts... args) {
        queueEvent(createEvent<EventT>(args...));
    }

    template<typename EventT>
    void queueEvent(EventT &event) {
        if (m_eventListeners.count(EventT::TypeId()) > 0) {
            auto eventP = new RefCountEvent(&event);

            for (const auto &listener: m_eventListeners[EventT::TypeId()]) {
                listener->registerEvent(*eventP);
            }
        }
    }

    template<class EventT, class... Ts>
    EventT &createEvent(Ts... args) {
        auto evt = new EventT(args...);
        evt->m_EventManager = this;

        return *evt;
    }

private:
    std::map<BaseEvent::IdType, EventListenersList> m_eventListeners;
};

#endif
