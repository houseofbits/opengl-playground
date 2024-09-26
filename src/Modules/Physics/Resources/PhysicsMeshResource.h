#pragma once

#include "../../../Core/API.h"
#include "PhysicsResource.h"

class PhysicsMeshResource : public Resource {
public:
    PhysicsMeshResource();

    Resource::Status fetchData(ResourceManager &) override;
    Resource::Status build() override;
    void destroy() override;
    [[nodiscard]] JPH::TriangleList createTriangleMeshShape(glm::vec3 scale) const;
    [[nodiscard]] JPH::Array<JPH::Vec3> createConvexMeshShape(glm::vec3 scale) const;

    glm::vec3 *m_vertices;
    int m_numVertices;
    int *m_indices;
    int m_numIndices;

private:
    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
