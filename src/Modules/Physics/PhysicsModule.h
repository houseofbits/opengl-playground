#pragma once

#include "../../Core/API.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/PhysicsMeshComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Systems/PhysicsSystem.h"

class PhysicsModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<RigidBodyComponent>("RigidBodyComponent");
        ctx.registerComponent<CharacterControllerComponent>("CharacterControllerComponent");
        ctx.registerComponent<PhysicsMeshComponent>("PhysicsMeshComponent");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<PhysicsSystem>(1);
    };
};
