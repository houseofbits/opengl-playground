#include "./PhysicsModule.h"

void PhysicsModule::registerScriptableTypes(ScriptingManager &scriptingManager) {
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
        "connect", &PhysicsHingeJointComponent::requestConnectState,
        "connectToEntity", &PhysicsHingeJointComponent::connectToEntity,
        "connectToEntityTarget", &PhysicsHingeJointComponent::connectToEntityTarget,
        "disconnect", &PhysicsHingeJointComponent::requestDisconnectState,
        "isConnected", &PhysicsHingeJointComponent::isStateConnected
    );

    scriptingManager.registerComponentType<PhysicsSliderJointComponent>(
        "setMotorOff", &PhysicsSliderJointComponent::setMotorOff,
        "setMotorVelocity", &PhysicsSliderJointComponent::setMotorVelocity,
        "lockInPlace", &PhysicsSliderJointComponent::lockInPlace,
        "unLock", &PhysicsSliderJointComponent::unLock,
        "connect", &PhysicsSliderJointComponent::requestConnectState,
        "connectToEntity", &PhysicsSliderJointComponent::connectToEntity,
        "connectToEntityTarget", &PhysicsSliderJointComponent::connectToEntityTarget,
        "disconnect", &PhysicsSliderJointComponent::requestDisconnectState,
        "isConnected", &PhysicsSliderJointComponent::isStateConnected
    );

    scriptingManager.registerComponentType<PhysicsFixedJointComponent>(
        "connect", &PhysicsFixedJointComponent::requestConnectState,
        "connectToEntity", &PhysicsFixedJointComponent::connectToEntity,
        "connectToEntityTarget", &PhysicsFixedJointComponent::connectToEntityTarget,
        "disconnect", &PhysicsFixedJointComponent::requestDisconnectState,
        "isConnected", &PhysicsFixedJointComponent::isStateConnected
    );

    scriptingManager.registerComponentType<PhysicsDistanceJointComponent>(
        "connect", &PhysicsDistanceJointComponent::requestConnectState,
        "connectToEntity", &PhysicsDistanceJointComponent::connectToEntity,
        "connectToEntityTarget", &PhysicsDistanceJointComponent::connectToEntityTarget,
        "disconnect", &PhysicsDistanceJointComponent::requestDisconnectState,
        "isConnected", &PhysicsDistanceJointComponent::isStateConnected
    );

    scriptingManager.registerComponentType<PhysicsSwingTwistJointComponent>(
        "connect", &PhysicsSwingTwistJointComponent::requestConnectState,
        "connectToEntity", &PhysicsSwingTwistJointComponent::connectToEntity,
        "connectToEntityTarget", &PhysicsSwingTwistJointComponent::connectToEntityTarget,
        "disconnect", &PhysicsSwingTwistJointComponent::requestDisconnectState,
        "isConnected", &PhysicsSwingTwistJointComponent::isStateConnected
    );
}
