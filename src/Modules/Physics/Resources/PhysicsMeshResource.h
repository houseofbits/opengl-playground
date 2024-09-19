#pragma once

#include "../../../Core/API.h"
#include "PhysicsResource.h"
#include <bullet/btBulletDynamicsCommon.h>

class PhysicsMeshResource : public Resource {
public:
    PhysicsMeshResource();

    Resource::Status fetchData(ResourceManager &) override;
    Resource::Status build() override;
    void destroy() override;
    [[nodiscard]] btBvhTriangleMeshShape* createTriangleMeshShape(glm::vec3 scale) const;
    [[nodiscard]] btCollisionShape* createConvexMeshShape(glm::vec3 scale) const;

    glm::vec3 *m_vertices;
    int m_numVertices;
    int *m_indices;
    int m_numIndices;

private:
    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
