#pragma once

#include "../../../Core/API.h"

class EditorUIEvent : public BaseEvent {
    TYPE_DEFINITION(EditorUIEvent);

public:
    typedef enum Type {
        UNDEFINED,
        EDITOR_ENABLED,
        EDITOR_DISABLED,
        SAVE,
        TRIGGER_PROBE_RENDER,
        TOGGLE_RENDER_SHADED,
        TOGGLE_RENDER_PROBES,
        TOGGLE_RENDER_REFLECTIONS,
        TOGGLE_SIMULATION_DISABLED,
        TOGGLE_SIMULATION_ENABLED,
        RESET_TO_INITIAL_TRANSFORM,
    } Type;

    Type m_Type{UNDEFINED};
};