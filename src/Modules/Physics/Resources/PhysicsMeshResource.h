#pragma once

#include "../../../Core/API.h"
#include "cooking/PxTriangleMeshDesc.h"

class PhysicsMeshResource : public Resource {
public:
    PhysicsMeshResource();

    Resource::Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;

    physx::PxTriangleMeshDesc m_pxTriangleMeshDescriptor;

private:
    static physx::PxTriangleMeshDesc createTriangleMeshDescriptor(int numVertices, physx::PxVec3*vertices, int numIndices, const physx::PxU32* indices);
};
