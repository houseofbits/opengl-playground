#pragma once

#include "../../Window.h"
#include "Event.h"

class WindowEvent : public BaseEvent {
TYPE_DEFINITION(WindowEvent);

public:
    typedef enum Type {
        CREATE,
        RESIZE,
        OPENGL_CONTEXT_CREATED,
    } Type;

    WindowEvent() : BaseEvent(), eventType(CREATE), window(nullptr) {
    }

    WindowEvent(WindowEvent::Type type, Window *w) : eventType(type), window(w) {
    }

    Type eventType;
    Window *window;
};