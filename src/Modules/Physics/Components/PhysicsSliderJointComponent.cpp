#include "PhysicsSliderJointComponent.h"
#include "../Helpers/PhysicsTypeCast.h"

using namespace JPH;

PhysicsSliderJointComponent::PhysicsSliderJointComponent() : Component(),
                                                             BasePhysicsJoint(),
                                                             m_Joint(nullptr),
                                                             m_isLimitsEnabled(false),
                                                             m_axis(1, 0, 0),
                                                             m_limits(0.0),
                                                             m_PhysicsResource(),
                                                             m_isMotorSettingsEnabled(false),
                                                             m_motorForceLimit(0),
                                                             m_motorDamping(0),
                                                             m_motorFrequency(0) {
}

void PhysicsSliderJointComponent::serialize(nlohmann::json &j) {
    serializeLinkedEntity(j);
    j[ENTITY_KEY] = m_targetEntityName;

    j[ARE_LIMITS_ENABLED_KEY] = m_isLimitsEnabled;
    j[LIMITS_KEY] = m_limits;
    j[AXIS_KEY] = m_axis;
    if (m_isMotorSettingsEnabled) {
        j[ENABLE_MOTOR_SETTINGS_KEY] = m_isMotorSettingsEnabled;
        j[MOTOR_MAX_FORCE_KEY] = m_motorForceLimit;
        j[MOTOR_DAMPING_KEY] = m_motorDamping;
        j[MOTOR_FREQUENCY_KEY] = m_motorFrequency;
    }
}

void PhysicsSliderJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    deserializeLinkedEntity(j);
    m_targetEntityName = j.value(ENTITY_KEY, m_targetEntityName);
    setLinkedEntityName(m_targetEntityName);

    m_isLimitsEnabled = j.value(ARE_LIMITS_ENABLED_KEY, m_isLimitsEnabled);
    m_limits = j.value(LIMITS_KEY, m_limits);
    m_axis = j.value(AXIS_KEY, m_axis);
    m_isMotorSettingsEnabled = j.value(ENABLE_MOTOR_SETTINGS_KEY, m_isMotorSettingsEnabled);
    m_motorForceLimit = j.value(MOTOR_MAX_FORCE_KEY, m_motorForceLimit);
    m_motorDamping = j.value(MOTOR_DAMPING_KEY, m_motorDamping);
    m_motorFrequency = j.value(MOTOR_FREQUENCY_KEY, m_motorFrequency);

    resourceManager.request(m_PhysicsResource, "physics");
}

bool PhysicsSliderJointComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsSliderJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!areAllowedToConnect(bodyA, bodyB)) {
        return;
    }

    JPH::SliderConstraintSettings settings;
    settings.mAutoDetectPoint = true;
    settings.SetSliderAxis(PhysicsTypeCast::glmToJPH(m_axis));
    if (m_isLimitsEnabled) {
        settings.mLimitsMin = m_limits.x;
        settings.mLimitsMax = m_limits.y;
    }

    if (m_isMotorSettingsEnabled) {
        MotorSettings &motor_settings = m_Joint->GetMotorSettings();
        motor_settings.mSpringSettings.mFrequency = m_motorFrequency;
        motor_settings.mSpringSettings.mDamping = m_motorDamping;
        motor_settings.SetForceLimit(m_motorForceLimit);
    }

    m_Joint = dynamic_cast<JPH::SliderConstraint *>(settings.Create(*bodyA.m_physicsBody, *bodyB.m_physicsBody));

    m_PhysicsResource().getSystem().AddConstraint(m_Joint);
}

void PhysicsSliderJointComponent::release() {
    if (m_Joint != nullptr) {
        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        delete m_Joint;
        m_Joint = nullptr;
    }
}

bool PhysicsSliderJointComponent::isCreated() const {
    return m_Joint != nullptr;
}

void PhysicsSliderJointComponent::update() {
}

void PhysicsSliderJointComponent::activate() {
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody1()->GetID());
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody2()->GetID());
}

void PhysicsSliderJointComponent::setMotorVelocity(float velocity) const {
    m_Joint->SetMotorState(JPH::EMotorState::Velocity);
    m_Joint->SetTargetVelocity(velocity);
}

void PhysicsSliderJointComponent::setMotorOff() const {
    m_Joint->SetMotorState(JPH::EMotorState::Off);
    m_Joint->SetTargetVelocity(0);
}

float PhysicsSliderJointComponent::getUnitPosition() const {
    float pos = m_Joint->GetCurrentPosition();
    const float range = m_limits.y - m_limits.x;

    return (pos - m_limits.x) / range;
}

void PhysicsSliderJointComponent::lockInPlace() {
    m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(),
                                                     EMotionType::Static,
                                                     EActivation::DontActivate);
}

void PhysicsSliderJointComponent::unLock() {
    m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(),
                                                     EMotionType::Dynamic,
                                                     EActivation::Activate);
}
