#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsMeshResource.h"
#include "../Resources/PhysicsResource.h"
#include <PhysX/PxPhysics.h>

class RigidBodyComponent : public Component {
    TYPE_DEFINITION(RigidBodyComponent);
public:
    inline static const std::string MODEL_KEY = "model";

    RigidBodyComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    bool isReady() override;
    void updateBodyParameters();

    void create(TransformComponent& transform);
    void createMeshShape(TransformComponent& transform);
    void releaseShapes() const;

    physx::PxRigidDynamic* m_pxRigidBody;
    glm::mat4 m_initialTransform;
    float m_density;
    glm::vec2 m_friction;
    float m_restitution;

    ResourceHandle<PhysicsMeshResource> m_meshResource;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
