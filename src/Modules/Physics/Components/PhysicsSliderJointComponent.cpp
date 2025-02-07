#include "PhysicsSliderJointComponent.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

using namespace JPH;

PhysicsSliderJointComponent::PhysicsSliderJointComponent() : BasePhysicsJoint(),
                                                             m_Joint(nullptr),
                                                             m_isLimitsEnabled(false),
                                                             m_limits(0.0),
                                                             m_axis(1, 0, 0),
                                                             m_isMotorSettingsEnabled(false),
                                                             m_motorForceLimit(0),
                                                             m_motorDamping(0),
                                                             m_motorFrequency(0) {
}

void PhysicsSliderJointComponent::serialize(nlohmann::json &j) {
    BasePhysicsJoint::serialize(j);

    j[ENTITY_KEY_A] = m_targetEntityAName;
    j[ENTITY_KEY_B] = m_targetEntityBName;
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
    BasePhysicsJoint::deserialize(j, resourceManager);

    m_isLimitsEnabled = j.value(ARE_LIMITS_ENABLED_KEY, m_isLimitsEnabled);
    m_limits = j.value(LIMITS_KEY, m_limits);
    m_axis = j.value(AXIS_KEY, m_axis);
    m_isMotorSettingsEnabled = j.value(ENABLE_MOTOR_SETTINGS_KEY, m_isMotorSettingsEnabled);
    m_motorForceLimit = j.value(MOTOR_MAX_FORCE_KEY, m_motorForceLimit);
    m_motorDamping = j.value(MOTOR_DAMPING_KEY, m_motorDamping);
    m_motorFrequency = j.value(MOTOR_FREQUENCY_KEY, m_motorFrequency);
}

bool PhysicsSliderJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!areAllowedToConnect(bodyA, bodyB)) {
        return false;
    }

    auto builder = PhysicsBuilder::newJoint(m_PhysicsResource().getSystem())
            .setBodies(bodyA, bodyB);

    if (m_isLimitsEnabled) {
        builder.setLimits(m_limits);
    }
    if (m_isMotorSettingsEnabled) {
        builder.setMotorSettings(m_motorFrequency, m_motorDamping);
        builder.setMotorForceLimit(m_motorForceLimit);
    }

    m_Joint = builder.createSliderConstraint(m_axis);

    return true;
}

void PhysicsSliderJointComponent::release() {
    if (m_Joint != nullptr) {
        // Log::write("Release slider joint ", m_Joint);

        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        m_Joint = nullptr;
    }
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
