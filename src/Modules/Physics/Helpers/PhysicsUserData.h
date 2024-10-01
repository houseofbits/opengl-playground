#pragma once

#include "../../../Core/API.h"

class PhysicsUserData {
public:
    explicit PhysicsUserData(Identity::Type id) {
        m_entityId = id;
    }

    Identity::Type m_entityId;
};
