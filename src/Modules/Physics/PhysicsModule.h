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
#include "Editors/CharacterControllerComponentEdit.h"
#include "Editors/PhysicsBodyComponentEdit.h"
#include "Editors/PhysicsHingeJointComponentEdit.h"
#include "Editors/PhysicsSliderJointComponentEdit.h"
#include "Editors/PhysicsFixedJointComponentEdit.h"

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
        ctx.registerBehaviour<CharacterMouseLookBehaviour>();
        ctx.registerBehaviour<CharacterPickingBehaviour>();
    }

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<PhysicsSystem>(1);
        ctx.registerEntitySystem<JointsProcessingSystem>(2);
        ctx.registerEntitySystem<PhysicsBodyProcessingSystem>(3);
        ctx.registerEntitySystem<CharacterControllerSystem>(4);
    };

    void postRegister(EntityContext &ctx) override {
        auto editorSystem = ctx.getSystem<EditorUISystem>();
        if (editorSystem != nullptr) {
            editorSystem->registerComponentEditor<PhysicsCharacterComponent, CharacterControllerComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsBodyComponent, PhysicsBodyComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsHingeJointComponent, PhysicsHingeJointComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsSliderJointComponent, PhysicsSliderJointComponentEdit>();
            editorSystem->registerComponentEditor<PhysicsFixedJointComponent, PhysicsFixedJointComponentEdit>();
        }
    }
};


