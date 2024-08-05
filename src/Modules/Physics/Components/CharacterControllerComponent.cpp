#include "CharacterControllerComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/PhysicsSystem.h"

CharacterControllerComponent::CharacterControllerComponent() : Component(),
                                                               m_height(1.75),
                                                               m_radius(0.25),
                                                               m_CCTController(nullptr),
                                                               m_initialPosition() {
}

void CharacterControllerComponent::serialize(nlohmann::json &j) {
    j["height"] = m_height;
    j["radius"] = m_radius;
}

void CharacterControllerComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_height = j.value("height", m_height);
    m_radius = j.value("radius", m_radius);
}

void CharacterControllerComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<PhysicsSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}
glm::vec3 CharacterControllerComponent::getPhysicsPosition() const {
    if (m_CCTController != nullptr) {
        auto p = m_CCTController->getPosition();
        return {p.x, p.y, p.z};
    }
    return glm::vec3(0);
}
void CharacterControllerComponent::setPhysicsPosition(glm::vec3 position) {
    if (m_CCTController != nullptr) {
        m_initialPosition = position;
        m_CCTController->setPosition(physx::PxExtendedVec3(position.x, position.y, position.z));
    }
}
