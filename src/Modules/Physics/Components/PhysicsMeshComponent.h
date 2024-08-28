#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsMeshResource.h"

class PhysicsMeshComponent : public Component {
    TYPE_DEFINITION(PhysicsMeshComponent);
public:
    inline static const std::string MODEL_KEY = "model";

    PhysicsMeshComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    bool isReady() override;
    void create(TransformComponent& transform);
    void createMeshShape(TransformComponent& transform);
    void releaseShapes() const;

    physx::PxRigidStatic *m_staticBody;
    glm::vec2 m_friction;
    float m_restitution;
    ResourceHandle<PhysicsMeshResource> m_meshResource;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
