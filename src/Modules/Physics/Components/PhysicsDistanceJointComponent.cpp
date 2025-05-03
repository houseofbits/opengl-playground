#include "PhysicsDistanceJointComponent.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

PhysicsDistanceJointComponent::PhysicsDistanceJointComponent() : BasePhysicsJoint(), m_Joint(nullptr),
                                                                 m_limits(0.0),
                                                                 m_localAttachmentMatrixA(1.0),
                                                                 m_localAttachmentMatrixB(1.0) {
}

void PhysicsDistanceJointComponent::serialize(nlohmann::json &j) {
    BasePhysicsJoint::serialize(j);

    j[ENTITY_KEY_A] = m_targetEntityAName;
    j[ENTITY_KEY_B] = m_targetEntityBName;
    j[LIMITS_KEY] = m_limits;
    j[ATTACHMENT_A_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrixA);
    j[ATTACHMENT_B_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrixB);
}

void PhysicsDistanceJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    BasePhysicsJoint::deserialize(j, resourceManager);

    m_limits = j.value(LIMITS_KEY, m_limits);
    m_localAttachmentMatrixA = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_A_POSITION_KEY, glm::vec3(0)),
        glm::quat(1, 0, 0, 0)
    );

    m_localAttachmentMatrixB = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_B_POSITION_KEY, glm::vec3(0)),
        glm::quat(1, 0, 0, 0)
    );
}

bool PhysicsDistanceJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    m_Joint = PhysicsBuilder::newJoint(m_PhysicsResource().getSystem())
            .setAttachments(m_localAttachmentMatrixA, m_localAttachmentMatrixB)
            .setBodies(bodyA, bodyB)
            .setLimits(m_limits)
            .createDistanceConstraint();

    return true;
}

void PhysicsDistanceJointComponent::release() {
    if (m_Joint != nullptr) {
        bool exists = false;
        auto constr = m_PhysicsResource().getSystem().GetConstraints();
        for (auto &constraint : constr) {
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

void PhysicsDistanceJointComponent::update() {
}
