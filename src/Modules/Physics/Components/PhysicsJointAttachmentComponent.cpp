#include "PhysicsJointAttachmentComponent.h"
#include "../../../Core/Helper/Math.h"

PhysicsJointAttachmentComponent::PhysicsJointAttachmentComponent() : Component(), m_localAttachmentMatrix(1.0) {
}

void PhysicsJointAttachmentComponent::serialize(nlohmann::json &j) {
    j[ATTACHMENT_POSITION_KEY] = Math::getTranslation(m_localAttachmentMatrix);
    j[ATTACHMENT_ROTATION_KEY] = Math::getRotation(m_localAttachmentMatrix);
}

void PhysicsJointAttachmentComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_localAttachmentMatrix = Math::createMatrixFromTranslationAndRotation(
        j.value(ATTACHMENT_POSITION_KEY, glm::vec3(0)),
        j.value(ATTACHMENT_ROTATION_KEY, glm::quat(1, 0, 0, 0))
    );
}