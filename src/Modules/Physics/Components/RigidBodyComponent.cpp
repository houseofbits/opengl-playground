#include "RigidBodyComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/PhysicsSystem.h"

RigidBodyComponent::RigidBodyComponent() : Component(), m_pxRigidBody(nullptr) {
}

void RigidBodyComponent::serialize(nlohmann::json &j) {
}

void RigidBodyComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
}

void RigidBodyComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<PhysicsSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}
