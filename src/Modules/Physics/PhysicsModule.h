#pragma once

#include "../../Core/API.h"
#include "Components/PhysicsCharacterComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/PhysicsHingeJointComponent.h"
#include "Systems/CharacterControllerSystem.h"
#include "Systems/JointsProcessingSystem.h"
#include "Systems/PhysicsBodyProcessingSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Behaviours/CharacterMouseLookBehaviour.h"
#include "Behaviours/CharacterPickingBehaviour.h"
#include "Components/PhysicsSliderJointComponent.h"
#include "Components/PhysicsFixedJointComponent.h"
#include "Components/PhysicsTriggerShapeComponent.h"
#include "Behaviours/DoorActivationBehaviour.h"

class PhysicsModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<PhysicsCharacterComponent>("cct");
        ctx.registerComponent<PhysicsBodyComponent>("physicsBody");
        ctx.registerComponent<PhysicsHingeJointComponent>("physicsHingeJoint");
        ctx.registerComponent<PhysicsSliderJointComponent>("physicsSliderJoint");
        ctx.registerComponent<PhysicsFixedJointComponent>("physicsFixedJoint");
        ctx.registerComponent<PhysicsTriggerShapeComponent>("physicsTriggerShape");
    };

    void registerBehaviours(EntityContext & ctx) override {
        ctx.registerBehaviour<CameraControlInputBehaviour>();
        ctx.registerBehaviour<CharacterMouseLookBehaviour>();
        ctx.registerBehaviour<CharacterPickingBehaviour>();
        ctx.registerBehaviour<DoorActivationBehaviour>();
    }

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<PhysicsSystem>(1);
        ctx.registerEntitySystem<JointsProcessingSystem>(2);
        ctx.registerEntitySystem<PhysicsBodyProcessingSystem>(3);
        ctx.registerEntitySystem<CharacterControllerSystem>(4);
    };
};


