#include "PhysicsMeshResource.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include "../../../Renderer/Model/ModelGLTFLoader.h"
#include "../../../Renderer/Model/GLTFFileLoader.h"

PhysicsMeshResource::PhysicsMeshResource() : Resource(),
                                             m_model(),
                                             m_wireModel() {
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
    for (auto node: m_model.nodes) {
        for (unsigned int i = node.offset; i < node.offset + node.size; i += 3) {
            auto i1 = m_model.indices[i + 0];
            auto i2 = m_model.indices[i + 1];
            auto i3 = m_model.indices[i + 2];

            auto v1 = glm::vec4(m_model.vertices[i1].position, 1.0f) * node.modelMatrix;
            auto v2 = glm::vec4(m_model.vertices[i2].position, 1.0f) * node.modelMatrix;
            auto v3 = glm::vec4(m_model.vertices[i3].position, 1.0f) * node.modelMatrix;

            m_wireModel.addSegment(v1, v2);
            m_wireModel.addSegment(v2, v3);
            m_wireModel.addSegment(v3, v1);
        }
    }

    m_wireModel.build();

    return STATUS_READY;
}

void PhysicsMeshResource::destroy() {
}

JPH::TriangleList PhysicsMeshResource::createTriangleMeshShape(const glm::vec3 scale) {
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

JPH::Array<JPH::Vec3> PhysicsMeshResource::createConvexMeshShape(const glm::vec3 scale) {
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
