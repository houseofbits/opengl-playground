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
                                                           m_isMotorSettingsEnabled(false),
                                                           m_motorForceLimit(0),
                                                           m_motorDamping(0),
                                                           m_motorFrequency(0),
                                                           m_localAttachmentMatrixA(1.0),
                                                           m_localAttachmentMatrixB(1.0),
                                                           m_useStatefulJointBehaviour(false),
                                                           m_statefulJointBehaviour() {
}

void PhysicsHingeJointComponent::serialize(nlohmann::json &j) {
    BasePhysicsJoint::serialize(j);

    j[ARE_LIMITS_ENABLED_KEY] = m_isLimitsEnabled;
    j[ARE_LOCKING_LIMITS_ENABLED_KEY] = m_isLockingToLimitsEnabled;
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

    if (m_useStatefulJointBehaviour) {
        nlohmann::json statefulBehaviour;
        m_statefulJointBehaviour.serialize(statefulBehaviour);
        j[STATEFUL_BEHAVIOUR_KEY] = statefulBehaviour;
    }
}

void PhysicsHingeJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    BasePhysicsJoint::deserialize(j, resourceManager);

    m_isLimitsEnabled = j.value(ARE_LIMITS_ENABLED_KEY, m_isLimitsEnabled);
    m_isLockingToLimitsEnabled = j.value(ARE_LOCKING_LIMITS_ENABLED_KEY, m_isLockingToLimitsEnabled);
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

    if (j.contains(STATEFUL_BEHAVIOUR_KEY)) {
        m_useStatefulJointBehaviour = true;
        m_statefulJointBehaviour.deserialize(j[STATEFUL_BEHAVIOUR_KEY], resourceManager);
    }
}

bool PhysicsHingeJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
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

    return true;
}

void PhysicsHingeJointComponent::release() {
    if (m_Joint != nullptr) {
        Log::write("Delete joint from component");
        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        m_Joint = nullptr;
    }
    BasePhysicsJoint::release();
}

void PhysicsHingeJointComponent::activate() {
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody1()->GetID());
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody2()->GetID());
}

void PhysicsHingeJointComponent::update() {
    if (m_useStatefulJointBehaviour) {
        m_statefulJointBehaviour.processJoint(this);
    }
}

void PhysicsHingeJointComponent::setMotorVelocity(const float velocity) {
    m_Joint->SetMotorState(JPH::EMotorState::Velocity);
    m_Joint->SetTargetAngularVelocity(velocity);
    activate();
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
    float currentAngle = m_Joint->GetCurrentAngle();
    m_Joint->SetMotorState(EMotorState::Position);
    m_Joint->SetTargetAngle(currentAngle);

    MotorSettings motorSettings;
    // motorSettings.mFrequency = 30.0f;    // High frequency means it will hold very stiffly
    // motorSettings.mDamping = 1.0f;       // Prevent oscillations
    // motorSettings.mMaxForce = 1.0e6f;     // Large enough to resist any forces
    //
    // m_Joint->SetMotorSettings(motorSettings);

    // m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(),
    //                                                  EMotionType::Static,
    //                                                  EActivation::DontActivate);

}

void PhysicsHingeJointComponent::unLock() {
    // m_PhysicsResource().getInterface().SetMotionType(m_Joint->GetBody1()->GetID(),
    //                                                  EMotionType::Dynamic,
    //                                                  EActivation::Activate);
    m_Joint->SetMotorState(JPH::EMotorState::Off);
}
