#include "PhysicsHingeJointComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include <Jolt/Physics/Constraints/HingeConstraint.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/GroupFilterTable.h>

#include "../../../Core/Helper/Math.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

using namespace JPH;

PhysicsHingeJointComponent::PhysicsHingeJointComponent() : BasePhysicsJoint(),
                                                           m_isLimitsEnabled(false),
                                                           m_isLockingToLimitsEnabled(false),
                                                           m_Joint(nullptr),
                                                           m_angularLimits(0, 1.0),
                                                           // m_axisA(0, 1, 0),
                                                           // m_axisB(0, 1, 0),
                                                           // m_localAttachmentA(0, 0, 0),
                                                           // m_localAttachmentB(0, 0, 0),
                                                           m_isMotorSettingsEnabled(false),
                                                           m_motorForceLimit(0),
                                                           m_motorDamping(0),
                                                           m_motorFrequency(0),
                                                           m_localAttachmentMatrixA(1.0),
                                                           m_localAttachmentMatrixB(1.0) {
}

void PhysicsHingeJointComponent::serialize(nlohmann::json &j) {
    BasePhysicsJoint::serialize(j);

    j[ARE_LIMITS_ENABLED_KEY] = m_isLimitsEnabled;
    j[ARE_LOCKING_LIMITS_ENABLED_KEY] = m_isLockingToLimitsEnabled;
    // j[AXIS_A_KEY] = m_axisA;
    // j[AXIS_B_KEY] = m_axisB;
    // j[ATTACHMENT_A_KEY] = m_localAttachmentA;
    // j[ATTACHMENT_B_KEY] = m_localAttachmentB;
    j[LIMITS_KEY] = m_angularLimits;
    if (m_isMotorSettingsEnabled) {
        j[ENABLE_MOTOR_SETTINGS_KEY] = m_isMotorSettingsEnabled;
        j[MOTOR_MAX_FORCE_KEY] = m_motorForceLimit;
        j[MOTOR_DAMPING_KEY] = m_motorDamping;
        j[MOTOR_FREQUENCY_KEY] = m_motorFrequency;
    }

    j[ATTACHMENT_A_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrixA);
    j[ATTACHMENT_A_ROTATION_KEY] = Math::getRotation(m_localAttachmentMatrixA);
    j[ATTACHMENT_B_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrixB);
    j[ATTACHMENT_B_ROTATION_KEY] = Math::getRotation(m_localAttachmentMatrixB);
}

void PhysicsHingeJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    BasePhysicsJoint::deserialize(j, resourceManager);

    m_isLimitsEnabled = j.value(ARE_LIMITS_ENABLED_KEY, m_isLimitsEnabled);
    m_isLockingToLimitsEnabled = j.value(ARE_LOCKING_LIMITS_ENABLED_KEY, m_isLockingToLimitsEnabled);
    // m_axisA = j.value(AXIS_A_KEY, m_axisA);
    // m_axisB = j.value(AXIS_B_KEY, m_axisB);
    // m_localAttachmentA = j.value(ATTACHMENT_A_KEY, m_localAttachmentA);
    // m_localAttachmentB = j.value(ATTACHMENT_B_KEY, m_localAttachmentB);
    m_angularLimits = j.value(LIMITS_KEY, m_angularLimits);
    m_isMotorSettingsEnabled = j.value(ENABLE_MOTOR_SETTINGS_KEY, m_isMotorSettingsEnabled);
    m_motorForceLimit = j.value(MOTOR_MAX_FORCE_KEY, m_motorForceLimit);
    m_motorDamping = j.value(MOTOR_DAMPING_KEY, m_motorDamping);
    m_motorFrequency = j.value(MOTOR_FREQUENCY_KEY, m_motorFrequency);

    m_localAttachmentMatrixA = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_A_POSITION_KEY, glm::vec3(0)),
        j.value(ATTACHMENT_A_ROTATION_KEY, glm::quat(1, 0, 0, 0))
    );

    m_localAttachmentMatrixB = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_B_POSITION_KEY, glm::vec3(0)),
        j.value(ATTACHMENT_B_ROTATION_KEY, glm::quat(1, 0, 0, 0))
    );
}

bool PhysicsHingeJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!areAllowedToConnect(bodyA, bodyB)) {
        return false;
    }

    auto builder = PhysicsBuilder::newJoint(m_PhysicsResource().getSystem())
            .setAttachments(m_localAttachmentMatrixA, m_localAttachmentMatrixB)
            .setBodies(bodyA, bodyB);

    if (m_isLimitsEnabled) {
        builder.setLimits(m_angularLimits);
    }

    if (m_isMotorSettingsEnabled) {
        builder.setMotorSettings(m_motorFrequency, m_motorDamping);
        builder.setMotorForceLimit(m_motorForceLimit);
    }

    m_Joint = builder.createHingeConstraint();

    auto *groupFilter = new JPH::GroupFilterTable(2);
    groupFilter->DisableCollision(0, 1);
    m_Joint->GetBody1()->SetCollisionGroup(JPH::CollisionGroup(groupFilter, 0, 0));
    m_Joint->GetBody2()->SetCollisionGroup(JPH::CollisionGroup(groupFilter, 0, 1));

    bodyA.wakeUp();
    bodyB.wakeUp();

    return true;
}

void PhysicsHingeJointComponent::release() {
    if (m_Joint != nullptr) {
        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        m_Joint = nullptr;
    }
}

void PhysicsHingeJointComponent::activate() {
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody1()->GetID());
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody2()->GetID());
}

void PhysicsHingeJointComponent::update() {
}

void PhysicsHingeJointComponent::setMotorVelocity(float velocity) const {
    m_Joint->SetMotorState(JPH::EMotorState::Velocity);
    m_Joint->SetTargetAngularVelocity(velocity);
}

void PhysicsHingeJointComponent::setMotorOff() const {
    m_Joint->SetMotorState(JPH::EMotorState::Off);
    m_Joint->SetTargetAngularVelocity(0);
}

float PhysicsHingeJointComponent::getUnitPosition() const {
    float pos = m_Joint->GetCurrentAngle();
    const float range = m_angularLimits.y - m_angularLimits.x;

    return (pos - m_angularLimits.x) / range;
}

void PhysicsHingeJointComponent::lockInPlace() {
    m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(),
                                                     EMotionType::Static,
                                                     EActivation::DontActivate);
}

void PhysicsHingeJointComponent::unLock() {
    m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(),
                                                     EMotionType::Dynamic,
                                                     EActivation::Activate);
}
