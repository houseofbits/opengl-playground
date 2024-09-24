#pragma once

#include "../../../Core/API.h"

class PhysicsUserData {
public:
    explicit PhysicsUserData(Identity::Type id) {
        m_entityId = id;
        m_contactReporting = false;
    }

    Identity::Type m_entityId;
    bool  m_contactReporting;
};
