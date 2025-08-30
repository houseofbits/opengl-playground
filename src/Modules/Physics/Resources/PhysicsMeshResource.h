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

    [[nodiscard]] JPH::TriangleList getAllMeshTriangles(glm::vec3 scale) const;

    [[nodiscard]] JPH::Array<JPH::Vec3> getAllMeshVertices(glm::vec3 scale);

    [[nodiscard]] JPH::Array<JPH::Vec3> getMeshNodeVertices(const Model::MeshNode &node, glm::vec3 scale) const;

    ModelConfigurationLoader::ModelConfiguration m_model;

    WireModel m_wireModel;

private:
    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
