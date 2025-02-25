#pragma once

#include "../../../Core/API.h"

class PhysicsRayCastResult {
public:
    PhysicsRayCastResult() = default;

    Identity::Type m_entityId{};
    float m_distance{};
    glm::vec3 m_touchPoint{};
};
