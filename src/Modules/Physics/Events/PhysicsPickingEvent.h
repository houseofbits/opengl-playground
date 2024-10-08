#pragma once

#include "../../../Core/API.h"

class PhysicsPickingEvent : public BaseEvent {
TYPE_DEFINITION(PhysicsPickingEvent);

public:
    PhysicsPickingEvent() : BaseEvent() {}

    Identity::Type m_entityId{0};
    float m_distance{0};
    glm::vec3 m_touchPoint{0};
    bool m_doActivate{false};
};
