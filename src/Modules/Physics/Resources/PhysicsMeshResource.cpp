#include "PhysicsMeshResource.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include "../../../Renderer/Model/ModelGLTFLoader.h"
#include "../../../Renderer/Model/GLTFFileLoader.h"

PhysicsMeshResource::PhysicsMeshResource() : Resource(),
                                             m_model() {
}

Resource::Status PhysicsMeshResource::fetchData(ResourceManager &resourceManager) {
    addDependency("physics");
    resourceManager.request(m_PhysicsResource, "physics");

    auto model = GLTFFileLoader::loadModel(m_Path);
    if (model == nullptr) {
        return STATUS_FETCH_ERROR;
    }

    ModelConfigurationLoader::ModelConfiguration modelConfiguration;
    ModelConfigurationLoader::createFromGLTFModel(*model, m_model, {false, true});

    return STATUS_DATA_READY;
}

Resource::Status PhysicsMeshResource::build() {
    return STATUS_READY;
}

void PhysicsMeshResource::destroy() {
}

JPH::TriangleList PhysicsMeshResource::createTriangleMeshShape(glm::vec3 scale) const {
    Log::write("Create mesh ", m_model.indices.size());
    JPH::TriangleList triangles;
    for (int i = 0; i < m_model.indices.size(); i += 3) {
        JPH::Vec3 pv[3];
        for (int p = 0; p < 3; p++) {
            const auto index = m_model.indices[i + p];
            const auto vertex = m_model.vertices[index].position;
            pv[p] = {
                vertex.x * scale.x,
                vertex.y * scale.y,
                vertex.z * scale.z
            };
        }

        triangles.push_back(JPH::Triangle(pv[0], pv[1], pv[2]));
    }

    return triangles;
}

JPH::Array<JPH::Vec3> PhysicsMeshResource::createConvexMeshShape(glm::vec3 scale) const {
    JPH::Array<JPH::Vec3> points;
    for (const auto &vertex: m_model.vertices) {
        const auto pos = vertex.position;
        points.push_back({
            pos.x * scale.x,
            pos.y * scale.y,
            pos.z * scale.z
        });
    }

    return points;
}
