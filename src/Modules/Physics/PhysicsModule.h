#pragma once

#include "Components/PhysicsCharacterComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/PhysicsHingeJointComponent.h"
#include "Systems/CharacterControllerSystem.h"
#include "Systems/JointsProcessingSystem.h"
#include "Systems/PhysicsBodyProcessingSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Components/PhysicsSliderJointComponent.h"
#include "Components/PhysicsFixedJointComponent.h"
#include "Editors/CharacterControllerComponentEdit.h"
#include "Editors/PhysicsBodyComponentEdit.h"
#include "Editors/PhysicsHingeJointComponentEdit.h"
#include "Editors/PhysicsSliderJointComponentEdit.h"
#include "Editors/PhysicsFixedJointComponentEdit.h"
#include "Editors/PhysicsShapeComponentEdit.h"
#include "../Editor/Systems/EditorUISystem.h"
#include "Editors/PhysicsHingeJointComponentTransformEdit.h"
#include "Editors/PhysicsSliderJointComponentTransformEdit.h"
#include "Events/PhysicsPickingEvent.h"
#include "Events/PhysicsTriggerShapeEvent.h"
#include "Components/PhysicsShapeComponent.h"
#include "Editors/PhysicsShapeComponentTransformEdit.h"
#include "Events/PhysicsSensorEvent.h"

class PhysicsModule final : public EntityModule {
public:
    static constexpr EntitySystemRegistry::ProcessType PHYSICS_PROCESS = 2;

    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<PhysicsCharacterComponent>();
        ctx.registerComponent<PhysicsBodyComponent>();
        ctx.registerComponent<PhysicsHingeJointComponent>();
        ctx.registerComponent<PhysicsSliderJointComponent>();
        ctx.registerComponent<PhysicsFixedJointComponent>();
        ctx.registerComponent<PhysicsShapeComponent>();
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.entitySystemRegistry.createContinuousProcess(PHYSICS_PROCESS, 16);

        ctx.registerEntitySystem<PhysicsSystem>(PHYSICS_PROCESS, 1);
        ctx.registerEntitySystem<PhysicsBodyProcessingSystem>(PHYSICS_PROCESS, 2);
        ctx.registerEntitySystem<JointsProcessingSystem>(PHYSICS_PROCESS, 3);
        ctx.registerEntitySystem<CharacterControllerSystem>(PHYSICS_PROCESS, 4);
    }

    void registerScriptableTypes(ScriptingManager &scriptingManager) override {
        scriptingManager.registerEventType<PhysicsPickingEvent>("PhysicsPickingEvent",
                                                                "distance", &PhysicsPickingEvent::m_distance,
                                                                "entityName", &PhysicsPickingEvent::m_entityName,
                                                                "shapeName", &PhysicsPickingEvent::m_shapeComponentName
        );
        scriptingManager.registerEventType<PhysicsSensorEvent>("PhysicsSensorEvent",
                                                               "colliderName",
                                                               &PhysicsSensorEvent::m_colliderEntityName,
                                                               "sensorName",
                                                               &PhysicsSensorEvent::m_sensorEntityName,
                                                               "sensorShapeName",
                                                               &PhysicsSensorEvent::m_sensorShapeName
        );

        scriptingManager.registerComponentType<PhysicsHingeJointComponent>(
            "setMotorOff", &PhysicsHingeJointComponent::setMotorOff,
            "setMotorVelocity", &PhysicsHingeJointComponent::setMotorVelocity,
            "lockInPlace", &PhysicsHingeJointComponent::lockInPlace,
            "unLock", &PhysicsHingeJointComponent::unLock,
            "toggleState", [](PhysicsHingeJointComponent& self) {
                if (self.m_useStatefulJointBehaviour) {
                    self.m_statefulJointBehaviour.toggleJointState(&self);
                }
            }
        );

        scriptingManager.registerComponentType<PhysicsSliderJointComponent>(
            "setMotorOff", &PhysicsSliderJointComponent::setMotorOff,
            "setMotorVelocity", &PhysicsSliderJointComponent::setMotorVelocity,
            "lockInPlace", &PhysicsSliderJointComponent::lockInPlace,
            "unLock", &PhysicsSliderJointComponent::unLock
            // "toggleState", [](PhysicsSliderJointComponent& self) {
            //     if (self.m_useStatefulJointBehaviour) {
            //         self.m_statefulJointBehaviour.toggleJointState(&self);
            //     }
            // }
        );
    }

    void postRegister(EntityContext &ctx) override {
        if (const auto editorSystem = ctx.getSystem<EditorUISystem>(); editorSystem != nullptr) {
            editorSystem->registerTransformComponentEditor<PhysicsHingeJointComponent,
                PhysicsHingeJointComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsSliderJointComponent,
                PhysicsSliderJointComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsShapeComponent,
                PhysicsShapeComponentTransformEdit>();

            editorSystem->registerComponentEditor<PhysicsBodyComponent>(processPhysicsBodyComponentEditor);
            editorSystem->registerComponentEditor<PhysicsShapeComponent>(processPhysicsShapeComponentEditor);
            editorSystem->registerComponentEditor<PhysicsCharacterComponent>(
                processPhysicsCharacterControllerComponentEditor);
            editorSystem->registerComponentEditor<PhysicsFixedJointComponent>(processPhysicsFixedJointComponentEditor);
            editorSystem->registerComponentEditor<PhysicsHingeJointComponent>(processPhysicsHingeJointComponentEditor);
            editorSystem->registerComponentEditor<
                PhysicsSliderJointComponent>(processPhysicsSliderJointComponentEditor);
        }
    }
};
