#pragma once

#include "../../../Core/API.h"

class PhysicsPickingEvent : public BaseEvent {
    TYPE_DEFINITION(PhysicsPickingEvent);

public:
    PhysicsPickingEvent() : BaseEvent() {}

    Identity::Type m_entityId{};
    float m_distance{};
    glm::vec3 m_touchPoint{};
    bool m_doActivate{false};
};
