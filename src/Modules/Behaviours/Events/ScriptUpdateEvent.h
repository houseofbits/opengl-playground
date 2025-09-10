#pragma once

#include "../../../Core/API.h"

class ScriptUpdateEvent : public BaseEvent {
    TYPE_DEFINITION(ScriptUpdateEvent);
public:
    ScriptUpdateEvent(float timestamp) : m_timestamp(timestamp) {
    }

    float m_timestamp;
};
