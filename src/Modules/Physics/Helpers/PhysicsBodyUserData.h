#pragma once

#include <utility>
#include "../../../Core/API.h"

class PhysicsBodyUserData {
public:
    explicit PhysicsBodyUserData(const Identity::Type id) {
        m_entityId = id;
    }

    Identity::Type m_entityId;
};
