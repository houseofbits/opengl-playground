#pragma once
#include "../../Window.h"
#include "Event.h"

class WindowEvent : public BaseEvent
{
    TYPE_DEFINITION(WindowEvent);

public:
    typedef enum Type
    {
        CREATE,
        RESIZE,
    } Type;

    WindowEvent(WindowEvent::Type eventType, Window *window) : eventType(eventType), window(window)
    {
    }

    Type eventType;
    Window *window;
};