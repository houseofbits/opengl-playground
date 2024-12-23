#pragma once

#include "../../Core/API.h"
#include "Components/PhysicsCharacterComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/PhysicsHingeJointComponent.h"
#include "Systems/CharacterControllerSystem.h"
#include "Systems/JointsProcessingSystem.h"
#include "Systems/PhysicsBodyProcessingSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/PhysicsTriggerShapeSystem.h"
#include "Components/PhysicsSliderJointComponent.h"
#include "Components/PhysicsFixedJointComponent.h"
#include "Components/PhysicsTriggerShapeComponent.h"
#include "Editors/CharacterControllerComponentEdit.h"
#include "Editors/PhysicsBodyComponentEdit.h"
#include "Editors/PhysicsHingeJointComponentEdit.h"
#include "Editors/PhysicsSliderJointComponentEdit.h"
#include "Editors/PhysicsFixedJointComponentEdit.h"
#include "Editors/PhysicsTriggerShapeComponentEdit.h"

class PhysicsModule : public EntityModule {
public:
    static constexpr EntitySystemRegistry::ProcessType PHYSICS_PROCESS = 2;

    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<PhysicsCharacterComponent>("cct");
        ctx.registerComponent<PhysicsBodyComponent>("physicsBody");
        ctx.registerComponent<PhysicsHingeJointComponent>("physicsHingeJoint");
        ctx.registerComponent<PhysicsSliderJointComponent>("physicsSliderJoint");
        ctx.registerComponent<PhysicsFixedJointComponent>("physicsFixedJoint");
        ctx.registerComponent<PhysicsTriggerShapeComponent>("physicsTriggerShape");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.entitySystemRegistry.createContinuousProcess(PHYSICS_PROCESS, 16);

        ctx.registerEntitySystem_v2<PhysicsSystem>(PHYSICS_PROCESS, 1);
        ctx.registerEntitySystem_v2<JointsProcessingSystem>(PHYSICS_PROCESS, 2);
        ctx.registerEntitySystem_v2<PhysicsBodyProcessingSystem>(PHYSICS_PROCESS, 3);
        ctx.registerEntitySystem_v2<CharacterControllerSystem>(PHYSICS_PROCESS, 4);
        ctx.registerEntitySystem_v2<PhysicsTriggerShapeSystem>(PHYSICS_PROCESS, 5);
    };

    void postRegister(EntityContext &ctx) override {
        auto editorSystem = ctx.getSystem<EditorUISystem>();
        if (editorSystem != nullptr) {
            editorSystem->registerComponentEditor<PhysicsCharacterComponent, CharacterControllerComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsBodyComponent, PhysicsBodyComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsHingeJointComponent, PhysicsHingeJointComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsSliderJointComponent, PhysicsSliderJointComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsFixedJointComponent, PhysicsFixedJointComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsTriggerShapeComponent, PhysicsTriggerShapeComponentEdit>();
        }
    }
};
