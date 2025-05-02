#include "PhysicsFixedJointComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Helpers/Builder/PhysicsBuilder.h"

PhysicsFixedJointComponent::PhysicsFixedJointComponent() : BasePhysicsJoint(),
                                                           m_Joint(nullptr),
                                                           m_localAttachmentMatrixA(1.0),
                                                           m_localAttachmentMatrixB(1.0) {
}

void PhysicsFixedJointComponent::serialize(nlohmann::json &j) {
    BasePhysicsJoint::serialize(j);

    j[ENTITY_KEY_A] = m_targetEntityAName;
    j[ENTITY_KEY_B] = m_targetEntityBName;

    j[ATTACHMENT_A_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrixA);
    j[ATTACHMENT_B_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrixB);
}

void PhysicsFixedJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    BasePhysicsJoint::deserialize(j, resourceManager);

    m_localAttachmentMatrixA = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_A_POSITION_KEY, glm::vec3(0)),
        glm::quat(1, 0, 0, 0)
    );

    m_localAttachmentMatrixB = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_B_POSITION_KEY, glm::vec3(0)),
        glm::quat(1, 0, 0, 0)
    );
}

bool PhysicsFixedJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    m_Joint = PhysicsBuilder::newJoint(m_PhysicsResource().getSystem())
            .setAttachments(m_localAttachmentMatrixA, m_localAttachmentMatrixB)
            .setBodies(bodyA, bodyB)
            .createFixedConstraint();

    return true;
}

void PhysicsFixedJointComponent::release() {
    if (m_Joint != nullptr) {
        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        m_Joint = nullptr;
    }
    BasePhysicsJoint::release();
}

void PhysicsFixedJointComponent::update() {
}
