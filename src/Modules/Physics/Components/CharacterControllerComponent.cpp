#include "CharacterControllerComponent.h"
#include "../../../Core/Helper/Types.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/CharacterControllerSystem.h"
#include "../Systems/PhysicsSystem.h"

CharacterControllerComponent::CharacterControllerComponent() : Component(),
                                                               m_height(1.75),
                                                               m_radius(0.25),
                                                               m_CCTController(nullptr),
                                                               m_initialPosition(),
                                                               m_PhysicsResource() {
}

void CharacterControllerComponent::serialize(nlohmann::json &j) {
    j["height"] = m_height;
    j["radius"] = m_radius;
}

void CharacterControllerComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_height = j.value("height", m_height);
    m_radius = j.value("radius", m_radius);

    resourceManager.request(m_PhysicsResource, "physics");
}

void CharacterControllerComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<CharacterControllerSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

glm::vec3 CharacterControllerComponent::getPhysicsPosition() const {
    if (m_CCTController != nullptr) {
        auto p = m_CCTController->getPosition();
        return {p.x, p.y - (m_height * 0.5), p.z};
    }
    return glm::vec3(0);
}

void CharacterControllerComponent::setPhysicsPosition(glm::vec3 position) {
    if (m_CCTController != nullptr) {
        m_initialPosition = position;
        m_CCTController->setPosition(physx::PxExtendedVec3(position.x, position.y, position.z));
    }
}

void CharacterControllerComponent::create(TransformComponent &transform) {
    physx::PxCapsuleControllerDesc desc;
    desc.scaleCoeff = 1;
    desc.position = Types::GLMtoPxExtendedVec3(transform.getTranslation());
    desc.contactOffset = 0.001;
    desc.stepOffset = 0.001;
    desc.slopeLimit = 0;
    desc.radius = m_radius;
    desc.height = m_height - (m_radius * 2.0f);
    desc.upDirection = physx::PxVec3(0, 1, 0);
    desc.material = m_PhysicsResource().m_pxPhysics->createMaterial(0.5, 0.5, 0.5);
    desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;

    m_CCTController = m_PhysicsResource().m_ControllerManager->createController(desc);
    setPhysicsPosition(transform.getTranslation());
}

bool CharacterControllerComponent::isReady() {
    return m_PhysicsResource.isReady();
}
