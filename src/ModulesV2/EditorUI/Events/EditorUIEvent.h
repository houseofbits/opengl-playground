#pragma once

#include "../../../CoreV2/API.h"

class EditorUIEvent : public BaseEvent {
    TYPE_DEFINITION(EditorUIEvent);

public:
    typedef enum Type {
        UNDEFINED,
        EDITOR_ENABLED,
        EDITOR_DISABLED,
        SAVE
    } Type;

    Type m_Type{UNDEFINED};
};