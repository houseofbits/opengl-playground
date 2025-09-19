#pragma once

#include "../../../Core/API.h"

class PhysicsPickingEvent : public BaseEvent {
    TYPE_DEFINITION(PhysicsPickingEvent);

private:
    PhysicsPickingEvent() : BaseEvent() {
    }

public:
    PhysicsPickingEvent(const Identity::Type entityId, std::string entityName,
                        const float distance,
                        const glm::vec3 point,
                        const bool activate,
                        const Identity::Type shapeComponentId,
                        const std::string &shapeName) : BaseEvent(),
                                                        m_entityId(entityId),
                                                        m_entityName(entityName),
                                                        m_distance(distance),
                                                        m_touchPoint(point),
                                                        m_doActivate(activate),
                                                        m_shapeComponentId(shapeComponentId),
                                                        m_shapeComponentName(shapeName) {
    }

    Identity::Type m_entityId{0};
    std::string m_entityName;
    float m_distance{0};
    glm::vec3 m_touchPoint{0};
    bool m_doActivate{false};
    Identity::Type m_shapeComponentId{0};
    std::string m_shapeComponentName;
};
