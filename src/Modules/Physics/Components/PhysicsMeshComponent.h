#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsMeshResource.h"

class PhysicsMeshComponent : public Component {
    TYPE_DEFINITION(PhysicsMeshComponent);
public:
    inline static const std::string MODEL_KEY = "model";
    inline static const std::string TYPE_KEY = "type";

    enum MeshType {
        CONVEX_MESH_TYPE,
        TRIANGLE_MESH_TYPE
    };

    PhysicsMeshComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    bool isReady() override;
    void create(TransformComponent& transform);
    void createMeshShape(TransformComponent& transform);
    MeshType m_MeshType;

    physx::PxRigidStatic *m_staticBody;
    glm::vec2 m_friction;
    float m_restitution;
    ResourceHandle<PhysicsMeshResource> m_meshResource;
    ResourceHandle<PhysicsResource> m_PhysicsResource;

private:
    physx::PxShape* createConvexMeshShape(glm::vec3 scale);
    physx::PxShape* createTriangleMeshShape(glm::vec3 scale);
    void releaseShapes() const;
    [[nodiscard]] std::string getMeshTypeName() const;
    void getMeshTypeFromName(const std::string& name);
};
