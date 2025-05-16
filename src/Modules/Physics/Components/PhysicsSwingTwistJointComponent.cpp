#include "PhysicsSwingTwistJointComponent.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

PhysicsSwingTwistJointComponent::PhysicsSwingTwistJointComponent() : BasePhysicsJoint(), m_Joint(),
                                                                     m_localAttachmentMatrixA(1.0),
                                                                     m_localAttachmentMatrixB(1.0),
                                                                     m_twistAngleLimits(0, 0),
                                                                     m_coneHalfAngle(JPH::JPH_PI / 2, JPH::JPH_PI / 2),
                                                                     m_isSwingMotorSettingsEnabled(false),
                                                                     m_swingMotorDamping(1.0),
                                                                     m_swingMotorFrequency(100),
                                                                     m_useLookAtBehaviour(false),
                                                                     m_lookAtBehaviour() {
}

void PhysicsSwingTwistJointComponent::serialize(nlohmann::json &j) {
    BasePhysicsJoint::serialize(j);

    j[ATTACHMENT_A_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrixA);
    j[ATTACHMENT_A_ROTATION_KEY] = Math::getRotation(m_localAttachmentMatrixA);
    j[ATTACHMENT_B_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrixB);
    j[ATTACHMENT_B_ROTATION_KEY] = Math::getRotation(m_localAttachmentMatrixB);
    j[TWIST_LIMITS_KEY] = m_twistAngleLimits;
    j[CONE_HALF_ANGLES_KEY] = m_coneHalfAngle;

    j[ENABLE_SWING_MOTOR_SETTINGS_KEY] = m_isSwingMotorSettingsEnabled;
    if (m_isSwingMotorSettingsEnabled) {
        j[SWING_MOTOR_DAMPING_KEY] = m_swingMotorDamping;
        j[SWING_MOTOR_FREQUENCY_KEY] = m_swingMotorFrequency;
    }

    j[LOOK_AT_BEHAVIOUR_KEY] = m_useLookAtBehaviour;
    if (m_useLookAtBehaviour) {
        m_lookAtBehaviour.serialize(j);
    }
}

void PhysicsSwingTwistJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    BasePhysicsJoint::deserialize(j, resourceManager);

    m_localAttachmentMatrixA = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_A_POSITION_KEY, glm::vec3(0)),
        j.value(ATTACHMENT_A_ROTATION_KEY, glm::quat(1, 0, 0, 0))
    );

    m_localAttachmentMatrixB = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_B_POSITION_KEY, glm::vec3(0)),
        j.value(ATTACHMENT_B_ROTATION_KEY, glm::quat(1, 0, 0, 0))
    );

    m_twistAngleLimits = j.value(TWIST_LIMITS_KEY, m_twistAngleLimits);
    m_coneHalfAngle = j.value(CONE_HALF_ANGLES_KEY, m_coneHalfAngle);

    m_isSwingMotorSettingsEnabled = j.value(ENABLE_SWING_MOTOR_SETTINGS_KEY, m_isSwingMotorSettingsEnabled);
    if (m_isSwingMotorSettingsEnabled) {
        m_swingMotorDamping = j.value(SWING_MOTOR_DAMPING_KEY, m_swingMotorDamping);
        m_swingMotorFrequency = j.value(SWING_MOTOR_FREQUENCY_KEY, m_swingMotorFrequency);
    }

    m_useLookAtBehaviour = j.value(LOOK_AT_BEHAVIOUR_KEY, m_useLookAtBehaviour);
    if (m_useLookAtBehaviour) {
        m_lookAtBehaviour.deserialize(j, resourceManager);
    }
}

void PhysicsSwingTwistJointComponent::update() {
    if (m_useLookAtBehaviour) {
        m_lookAtBehaviour.processSwingTwistJoint(this);
    }
}

bool PhysicsSwingTwistJointComponent::create(PhysicsComponent &bodyA, PhysicsComponent &bodyB) {
    auto builder = PhysicsBuilder::newJoint(m_PhysicsResource().getSystem())
            .setAttachments(m_localAttachmentMatrixA, m_localAttachmentMatrixB)
            .setTwistAngleLimits(m_twistAngleLimits)
            .setConeHalfAngle(m_coneHalfAngle)
            .setBodies(bodyA.getId(), bodyB.getId());

    if (m_isSwingMotorSettingsEnabled) {
        builder.setSwingMotorSettings(m_swingMotorFrequency, m_swingMotorDamping);
    }

    m_Joint = builder.createSwingTwistConstraint();

    return true;
}

void PhysicsSwingTwistJointComponent::release() {
    if (m_Joint != nullptr) {
        bool exists = false;
        auto constr = m_PhysicsResource().getSystem().GetConstraints();
        for (auto &constraint: constr) {
            if (constraint.GetPtr() == m_Joint.GetPtr()) {
                exists = true;
                break;
            }
        }
        if (exists) {
            m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        }

        m_Joint = nullptr;
    }
    BasePhysicsJoint::release();
}

void PhysicsSwingTwistJointComponent::activate() {
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody1()->GetID());
    m_PhysicsResource().getInterface().ActivateBody(m_Joint->GetBody2()->GetID());
}
