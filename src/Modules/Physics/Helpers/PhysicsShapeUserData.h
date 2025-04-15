#pragma once

#include <utility>
#include "../../../Core/API.h"

class PhysicsShapeUserData {
public:
    explicit PhysicsShapeUserData(const Identity::Type id, std::string name) {
        m_physicsShapeComponentId = id;
        m_physicsShapeComponentName = std::move(name);
    }

    Identity::Type m_physicsShapeComponentId;
    std::string m_physicsShapeComponentName;
};
