#pragma once

#include "../../../Core/API.h"

class CharacterPickingEvent : public BaseEvent {
    TYPE_DEFINITION(CharacterPickingEvent);

public:
    CharacterPickingEvent() : BaseEvent() {}

    Identity::Type m_entityId{};
    float m_distance{};
    glm::vec3 m_touchPoint{};
    bool m_doActivate{false};
};
