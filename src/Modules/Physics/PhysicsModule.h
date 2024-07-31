#pragma once

#include "../../Core/API.h"
#include "Systems/PhysicsSystem.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

class PhysicsModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {

    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<PhysicsSystem>(1);
    };
};
