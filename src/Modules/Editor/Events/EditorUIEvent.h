#pragma once

#include "../../../Core/API.h"

class EditorUIEvent : public BaseEvent {
TYPE_DEFINITION(EditorUIEvent);

public:
    typedef enum Type {
        UNDEFINED,
        EDITOR_ENABLED,
        EDITOR_DISABLED,
        LOAD,
        SAVE,
        TRIGGER_PROBE_RENDER,
        TOGGLE_RENDER_SHADED,
        TOGGLE_RENDER_PROBES,
        TOGGLE_RENDER_REFLECTIONS,
        TOGGLE_RENDER_PHYSICS,
        TOGGLE_SIMULATION_DISABLED,
        TOGGLE_SIMULATION_ENABLED,
        RESET_TO_INITIAL_TRANSFORM,
    } Type;

    EditorUIEvent() : BaseEvent() {
    }

    explicit EditorUIEvent(EditorUIEvent::Type type) : BaseEvent(), m_Type(type) {}

    Type m_Type{UNDEFINED};
};