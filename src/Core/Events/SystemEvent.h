#pragma once

#include "../../Window.h"
#include "Event.h"

class SystemEvent : public BaseEvent {
TYPE_DEFINITION(SystemEvent);

public:
    typedef enum Type {
        WINDOW_CREATED,
        WINDOW_RESIZED,
        WINDOW_CONTEXT_CREATED,
        ENTITY_SYSTEMS_READY,
    } Type;

    SystemEvent() : BaseEvent(), eventType(WINDOW_CREATED), window(nullptr) {
    }

    explicit SystemEvent(SystemEvent::Type type) : eventType(type), window(nullptr) {
    }

    SystemEvent(SystemEvent::Type type, Window *w) : eventType(type), window(w) {
    }

    Type eventType;
    Window *window;
};