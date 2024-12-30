#include "PhysicsTriggerShapeComponent.h"
#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>
#include "../Helpers/PhysicsTypeCast.h"

using namespace JPH;

PhysicsTriggerShapeComponent::PhysicsTriggerShapeComponent() : Component(),
                                                               m_PhysicsResource(),
                                                               m_physicsBody(nullptr) {
}

void PhysicsTriggerShapeComponent::serialize(nlohmann::json &j) {
}

void PhysicsTriggerShapeComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    resourceManager.request(m_PhysicsResource, "physics");
}

bool PhysicsTriggerShapeComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsTriggerShapeComponent::create(TransformComponent &transform) {
    const glm::vec3 scale = transform.getScale();
    const JPH::Vec3 halfExtents(scale.x / 2.0f, scale.y / 2.0f, scale.z / 2.0f);

    const auto material = new JPH::PhysicsMaterialSimple("Material2", JPH::Color(255, 0, 0));
    const Ref<JPH::BoxShape> shape = new JPH::BoxShape(halfExtents, cDefaultConvexRadius, material);

    JPH::BodyCreationSettings settings(
        shape,
        PhysicsTypeCast::glmToJPH(transform.getWorldPosition()),
        PhysicsTypeCast::glmToJPH(transform.getRotation()),
        JPH::EMotionType::Static,
        Layers::SENSOR
    );

    settings.mIsSensor = true;

    m_physicsBody = m_PhysicsResource().getInterface().CreateBody(settings);

    auto *userData = new PhysicsUserData(m_EntityId.id());
    m_physicsBody->SetUserData(reinterpret_cast<unsigned long>(userData));

    m_PhysicsResource().getInterface().AddBody(m_physicsBody->GetID(), JPH::EActivation::DontActivate);
}

bool PhysicsTriggerShapeComponent::isCreated() const {
    return m_physicsBody != nullptr;
}
