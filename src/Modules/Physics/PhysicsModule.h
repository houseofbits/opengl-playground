#pragma once

#include "../../Core/API.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/HingeJointComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Systems/CharacterControllerSystem.h"
#include "Systems/PhysicsBodyProcessingSystem.h"
#include "Systems/PhysicsSystem.h"

class PhysicsModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<CharacterControllerComponent>("CharacterControllerComponent");
        ctx.registerComponent<PhysicsBodyComponent>("PhysicsBodyComponent");
        ctx.registerComponent<HingeJointComponent>("HingeJointComponent");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<PhysicsSystem>(1);
        ctx.registerEntitySystem<PhysicsBodyProcessingSystem>(2);
        ctx.registerEntitySystem<CharacterControllerSystem>(3);
    };
};
