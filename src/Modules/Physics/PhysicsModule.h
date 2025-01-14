#pragma once

#include "../../Core/API.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/PhysicsJointComponent.h"
#include "Systems/CharacterControllerSystem.h"
#include "Systems/JointsProcessingSystem.h"
#include "Systems/PhysicsBodyProcessingSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Behaviours/CharacterMouseLookBehaviour.h"
#include "Behaviours/CharacterPickingBehaviour.h"

class PhysicsModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<CharacterControllerComponent>("cct");
        ctx.registerComponent<PhysicsBodyComponent>("physicsBody");
        ctx.registerComponent<PhysicsJointComponent>("physicsJoint");
    };

    void registerBehaviours(EntityContext & ctx) override {
        ctx.registerBehaviour<CameraControlInputBehaviour>();
        ctx.registerBehaviour<CharacterMouseLookBehaviour>();
        ctx.registerBehaviour<CharacterPickingBehaviour>();
    }

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<PhysicsSystem>(1);
        ctx.registerEntitySystem<JointsProcessingSystem>(2);
        ctx.registerEntitySystem<PhysicsBodyProcessingSystem>(3);
        ctx.registerEntitySystem<CharacterControllerSystem>(4);
    };
};
