#pragma once

#include "../../../Core/API.h"

class PhysicsPickingEvent : public BaseEvent {
TYPE_DEFINITION(PhysicsPickingEvent);

private:
    PhysicsPickingEvent() : BaseEvent() {}

public:
    PhysicsPickingEvent(Identity::Type entityId, float distance, glm::vec3 point, bool activate) : BaseEvent(),
                                                                                                   m_entityId(entityId),
                                                                                                   m_distance(distance),
                                                                                                   m_touchPoint(point),
                                                                                                   m_doActivate(
                                                                                                           activate) {}

    Identity::Type m_entityId{0};
    float m_distance{0};
    glm::vec3 m_touchPoint{0};
    bool m_doActivate{false};
};
