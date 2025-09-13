#pragma once

#include "Components/PhysicsCharacterComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/PhysicsHingeJointComponent.h"
#include "Systems/CharacterControllerSystem.h"
#include "Systems/JointsProcessingSystem.h"
#include "Systems/PhysicsBodyProcessingSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Components/PhysicsSliderJointComponent.h"
#include "Components/PhysicsDistanceJointComponent.h"
#include "Components/PhysicsSwingTwistJointComponent.h"
#include "Components/PhysicsFixedJointComponent.h"
#include "Editors/CharacterControllerComponentEdit.h"
#include "Editors/PhysicsBodyComponentEdit.h"
#include "Editors/PhysicsHingeJointComponentEdit.h"
#include "Editors/PhysicsSliderJointComponentEdit.h"
#include "Editors/PhysicsFixedJointComponentEdit.h"
#include "Editors/PhysicsDistanceJointComponentEdit.h"
#include "Editors/PhysicsSwingTwistJointComponentEdit.h"
#include "Editors/PhysicsShapeComponentEdit.h"
#include "../Editor/Systems/EditorUISystem.h"
#include "Components/PhysicsJointAttachmentComponent.h"
#include "Editors/PhysicsHingeJointComponentTransformEdit.h"
#include "Editors/PhysicsSliderJointComponentTransformEdit.h"
#include "Editors/PhysicsDistanceJointComponentTransformEdit.h"
#include "Editors/PhysicsFixedJointComponentTransformEdit.h"
#include "Editors/PhysicsSwingTwistJointComponentTransformEdit.h"
#include "Editors/PhysicsJointAttachmentComponentTransformEdit.h"
#include "Events/PhysicsPickingEvent.h"
#include "Events/PhysicsTriggerShapeEvent.h"
#include "Components/PhysicsShapeComponent.h"
#include "Editors/PhysicsJointAttachmentComponentEdit.h"
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
        ctx.registerComponent<PhysicsDistanceJointComponent>();
        ctx.registerComponent<PhysicsSwingTwistJointComponent>();
        ctx.registerComponent<PhysicsJointAttachmentComponent>();
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.entitySystemRegistry.createContinuousProcess(PHYSICS_PROCESS, 16);

        ctx.registerEntitySystem<PhysicsSystem>(PHYSICS_PROCESS, 1);
        ctx.registerEntitySystem<PhysicsBodyProcessingSystem>(PHYSICS_PROCESS, 2);
        ctx.registerEntitySystem<JointsProcessingSystem>(PHYSICS_PROCESS, 3);
        ctx.registerEntitySystem<CharacterControllerSystem>(PHYSICS_PROCESS, 4);
    }

    void registerScriptableTypes(ScriptingManager &scriptingManager) override {
        // scriptingManager.registerEventType<PhysicsPickingEvent>("PhysicsPickingEvent",
        //                                                         "distance", &PhysicsPickingEvent::m_distance,
        //                                                         "entityName", &PhysicsPickingEvent::m_entityName,
        //                                                         "shapeName", &PhysicsPickingEvent::m_shapeComponentName
        // );
        // scriptingManager.registerEventType<PhysicsSensorEvent>("PhysicsSensorEvent",
        //                                                        "colliderName",
        //                                                        &PhysicsSensorEvent::m_colliderEntityName,
        //                                                        "sensorName",
        //                                                        &PhysicsSensorEvent::m_sensorEntityName,
        //                                                        "sensorShapeName",
        //                                                        &PhysicsSensorEvent::m_sensorShapeName
        // );
        //
        //     scriptingManager.registerComponentType<PhysicsHingeJointComponent>(
        //         "setMotorOff", &PhysicsHingeJointComponent::setMotorOff,
        //         "setMotorVelocity", &PhysicsHingeJointComponent::setMotorVelocity,
        //         "lockInPlace", &PhysicsHingeJointComponent::lockInPlace,
        //         "unLock", &PhysicsHingeJointComponent::unLock,
        //         "connect", &PhysicsHingeJointComponent::requestConnectState,
        //         "connectToEntity", &PhysicsHingeJointComponent::connectToEntity,
        //         "connectToEntityTarget", &PhysicsHingeJointComponent::connectToEntityTarget,
        //         "disconnect", &PhysicsHingeJointComponent::requestDisconnectState,
        //         "isConnected", &PhysicsHingeJointComponent::isStateConnected,
        //         "toggleState", [](PhysicsHingeJointComponent &self) {
        //             if (self.m_useStatefulJointBehaviour) {
        //                 self.m_statefulJointBehaviour.toggleJointState(&self);
        //             }
        //         }
        //     );
        //
        // scriptingManager.registerComponentType<PhysicsSliderJointComponent>(
        //     "setMotorOff", &PhysicsSliderJointComponent::setMotorOff,
        //     "setMotorVelocity", &PhysicsSliderJointComponent::setMotorVelocity,
        //     "lockInPlace", &PhysicsSliderJointComponent::lockInPlace,
        //     "unLock", &PhysicsSliderJointComponent::unLock,
        //     "connect", &PhysicsSliderJointComponent::requestConnectState,
        //     "connectToEntity", &PhysicsSliderJointComponent::connectToEntity,
        //     "connectToEntityTarget", &PhysicsSliderJointComponent::connectToEntityTarget,
        //     "disconnect", &PhysicsSliderJointComponent::requestDisconnectState,
        //     "isConnected", &PhysicsSliderJointComponent::isStateConnected
        // );
        //
        // scriptingManager.registerComponentType<PhysicsFixedJointComponent>(
        //     "connect", &PhysicsFixedJointComponent::requestConnectState,
        //     "connectToEntity", &PhysicsFixedJointComponent::connectToEntity,
        //     "connectToEntityTarget", &PhysicsFixedJointComponent::connectToEntityTarget,
        //     "disconnect", &PhysicsFixedJointComponent::requestDisconnectState,
        //     "isConnected", &PhysicsFixedJointComponent::isStateConnected
        // );
        //
        // scriptingManager.registerComponentType<PhysicsDistanceJointComponent>(
        //     "connect", &PhysicsDistanceJointComponent::requestConnectState,
        //     "connectToEntity", &PhysicsDistanceJointComponent::connectToEntity,
        //     "connectToEntityTarget", &PhysicsDistanceJointComponent::connectToEntityTarget,
        //     "disconnect", &PhysicsDistanceJointComponent::requestDisconnectState,
        //     "isConnected", &PhysicsDistanceJointComponent::isStateConnected
        // );
        //
        // scriptingManager.registerComponentType<PhysicsSwingTwistJointComponent>(
        //     "connect", &PhysicsSwingTwistJointComponent::requestConnectState,
        //     "connectToEntity", &PhysicsSwingTwistJointComponent::connectToEntity,
        //     "connectToEntityTarget", &PhysicsSwingTwistJointComponent::connectToEntityTarget,
        //     "disconnect", &PhysicsSwingTwistJointComponent::requestDisconnectState,
        //     "isConnected", &PhysicsSwingTwistJointComponent::isStateConnected
        // );
    }

    void postRegister(EntityContext &ctx) override {
        if (const auto editorSystem = ctx.getSystem<EditorUISystem>(); editorSystem != nullptr) {
            editorSystem->registerTransformComponentEditor<PhysicsHingeJointComponent,
                PhysicsHingeJointComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsSliderJointComponent,
                PhysicsSliderJointComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsShapeComponent,
                PhysicsShapeComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsDistanceJointComponent,
                PhysicsDistanceJointComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsFixedJointComponent,
                PhysicsFixedJointComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsSwingTwistJointComponent,
                PhysicsSwingTwistJointComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<PhysicsJointAttachmentComponent,
                PhysicsJointAttachmentComponentTransformEdit>();

            editorSystem->registerComponentEditor<PhysicsBodyComponent>(processPhysicsBodyComponentEditor);
            editorSystem->registerComponentEditor<PhysicsJointAttachmentComponent>(
                processPhysicsJointAttachmentComponentEditor);
            editorSystem->registerComponentEditor<PhysicsShapeComponent>(processPhysicsShapeComponentEditor);
            editorSystem->registerComponentEditor<PhysicsCharacterComponent>(
                processPhysicsCharacterControllerComponentEditor);
            editorSystem->registerComponentEditor<PhysicsFixedJointComponent>(processPhysicsFixedJointComponentEditor);
            editorSystem->registerComponentEditor<PhysicsHingeJointComponent>(processPhysicsHingeJointComponentEditor);
            editorSystem->registerComponentEditor<
                PhysicsSliderJointComponent>(processPhysicsSliderJointComponentEditor);
            editorSystem->registerComponentEditor<
                PhysicsDistanceJointComponent>(processPhysicsDistanceJointComponentEditor);
            editorSystem->registerComponentEditor<
                PhysicsSwingTwistJointComponent>(processPhysicsSwingTwistJointComponentEditor);
        }
    }
};
