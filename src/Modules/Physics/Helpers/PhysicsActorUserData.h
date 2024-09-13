#pragma once

#include "../../../Core/API.h"

class PhysicsActorUserData {
public:
    explicit PhysicsActorUserData(Identity::Type id) {
        entityId = id;
    }

    Identity::Type entityId;
};
