#pragma once

#include "../../../Core/API.h"
#include "../Helpers/Window.h"

class SystemEvent : public BaseEvent {
TYPE_DEFINITION(SystemEvent);

public:
    typedef enum Type {
        WINDOW_CREATED,
        WINDOW_RESIZED,
        WINDOW_CONTEXT_CREATED,
        ENTITY_SYSTEMS_READY,
        REQUEST_GAME_MODE,
        REQUEST_EDITOR_MODE,
    } Type;

    SystemEvent() : BaseEvent(), eventType(WINDOW_CREATED), window(nullptr) {
    }

    explicit SystemEvent(const Type type) : eventType(type), window(nullptr) {
    }

    SystemEvent(const Type type, Window *w) : eventType(type), window(w) {
    }

    Type eventType;
    Window *window;
};
