#pragma once

#include "../../../Core/API.h"
#include "PhysicsResource.h"
#include "cooking/PxTriangleMeshDesc.h"

class PhysicsMeshResource : public Resource {
public:
    PhysicsMeshResource();

    Resource::Status fetchData(ResourceManager &) override;
    Resource::Status build() override;
    void destroy() override;

    physx::PxVec3 *m_vertices;
    int m_numVertices;
    physx::PxU32 *m_indices;
    int m_numIndices;

private:
    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
