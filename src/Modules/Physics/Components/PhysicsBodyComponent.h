#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsMeshResource.h"
#include "../Resources/PhysicsResource.h"

class PhysicsBodyComponent : public Component {
    TYPE_DEFINITION(PhysicsBodyComponent);
public:
    inline static const std::string MODEL_KEY = "model";
    inline static const std::string TYPE_KEY = "type";
    inline static const std::string SHAPE_KEY = "shape";
    inline static const std::string RESTITUTION_KEY = "restitution";
    inline static const std::string FRICTION_KEY = "friction";
    inline static const std::string DENSITY_KEY = "density";

    enum BodyType {
        BODY_TYPE_STATIC,
        BODY_TYPE_DYNAMIC,
    };

    enum MeshType {
        MESH_TYPE_CONVEX,
        MESH_TYPE_TRIANGLE,
    };

    PhysicsBodyComponent();
    ~PhysicsBodyComponent() override;

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    bool isReady() override;
    void create(TransformComponent& transform);
    void createMeshShape(TransformComponent& transform);
    void update(TransformComponent& transform, bool isSimulationEnabled) const;

    BodyType m_BodyType;
    MeshType m_MeshType;
    glm::vec2 m_friction;
    float m_restitution;
    float m_density;
    ResourceHandle<PhysicsMeshResource> m_meshResource;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    physx::PxRigidActor *m_pxRigidActor;

private:
    physx::PxShape* createConvexMeshShape(glm::vec3 scale);
    physx::PxShape* createTriangleMeshShape(glm::vec3 scale);
    void releaseShapes() const;
};
