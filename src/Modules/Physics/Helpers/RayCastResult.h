#pragma once

#include "../../../Core/API.h"

class RayCastResult {
public:
    RayCastResult() = default;

    Identity::Type m_entityId{};
    float m_distance{};
    glm::vec3 m_touchPoint{};
};
