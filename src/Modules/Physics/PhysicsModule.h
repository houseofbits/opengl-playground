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
#include "../Editor/Systems/EditorUISystem.h"
#include "Editors/PhysicsBodyComponentEdit.h"
#include "Editors/PhysicsHingeJointComponentTransformEdit.h"

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

        ctx.registerEntitySystem<PhysicsSystem>(PHYSICS_PROCESS, 1);
        ctx.registerEntitySystem<PhysicsBodyProcessingSystem>(PHYSICS_PROCESS, 2);
        ctx.registerEntitySystem<JointsProcessingSystem>(PHYSICS_PROCESS, 3);
        ctx.registerEntitySystem<CharacterControllerSystem>(PHYSICS_PROCESS, 4);
        ctx.registerEntitySystem<PhysicsTriggerShapeSystem>(PHYSICS_PROCESS, 5);
    };

    void postRegister(EntityContext &ctx) override {
        if (const auto editorSystem = ctx.getSystem<EditorUISystem>(); editorSystem != nullptr) {
            // editorSystem->registerTransformComponentEditor<PhysicsCharacterComponent, CharacterControllerComponentEdit>();
            // editorSystem->registerTransformComponentEditor<PhysicsBodyComponent, PhysicsBodyComponentEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsHingeJointComponent, PhysicsHingeJointComponentTransformEdit>();
            // editorSystem->registerTransformComponentEditor<PhysicsSliderJointComponent, PhysicsSliderJointComponentEdit>();
            // editorSystem->registerTransformComponentEditor<PhysicsFixedJointComponent, PhysicsFixedJointComponentEdit>();
            // editorSystem->registerTransformComponentEditor<PhysicsTriggerShapeComponent, PhysicsTriggerShapeComponentEdit>();

            editorSystem->registerComponentEditor<PhysicsBodyComponent>(processPhysicsBodyComponentEditor);
            editorSystem->registerComponentEditor<PhysicsCharacterComponent>(
                processPhysicsCharacterControllerComponentEditor);
            editorSystem->registerComponentEditor<PhysicsFixedJointComponent>(processPhysicsFixedJointComponentEditor);
            editorSystem->registerComponentEditor<PhysicsHingeJointComponent>(processPhysicsHingeJointComponentEditor);
            editorSystem->registerComponentEditor<
                PhysicsSliderJointComponent>(processPhysicsSliderJointComponentEditor);
            editorSystem->registerComponentEditor<PhysicsTriggerShapeComponent>(
                processPhysicsTriggerShapeComponentEditor);
        }
    }
};
