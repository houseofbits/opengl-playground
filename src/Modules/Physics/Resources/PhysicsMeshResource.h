#pragma once

#include "../../../Core/API.h"
#include "PhysicsResource.h"
#include "../../../Renderer/Model/ModelConfigurationLoader.h"
#include "../../../Renderer/Model/WireModel.h"

class PhysicsMeshResource : public Resource {
public:
    PhysicsMeshResource();

    Resource::Status fetchData(ResourceManager &) override;

    Resource::Status build() override;

    void destroy() override;

    [[nodiscard]] JPH::TriangleList createTriangleMeshShape(glm::vec3 scale);

    [[nodiscard]] JPH::Array<JPH::Vec3> createConvexMeshShape(glm::vec3 scale);

    ModelConfigurationLoader::ModelConfiguration m_model;

    WireModel m_wireModel;

private:
    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
