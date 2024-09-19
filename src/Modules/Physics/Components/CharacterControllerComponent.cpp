#include "CharacterControllerComponent.h"
#include "../../../Core/Helper/Types.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/CharacterControllerSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Helpers/FilterGroups.h"

//using namespace physx;

CharacterControllerComponent::CharacterControllerComponent() : Component(),
                                                               m_height(1.75),
                                                               m_radius(0.25),
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

void CharacterControllerComponent::create(TransformComponent &transform) {

}

bool CharacterControllerComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void CharacterControllerComponent::update(TransformComponent &transform, bool isSimulationEnabled) const {
    if (isSimulationEnabled) {
//        transform.setFromPxTransform(m_pxRigidActor->getGlobalPose());
    } else {
//        m_pxRigidActor->setGlobalPose(Types::GLMtoPxTransform(transform.getModelMatrix()));
    }
}