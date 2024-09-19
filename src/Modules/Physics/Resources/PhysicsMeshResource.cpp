#include "PhysicsMeshResource.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"
// #include "../foundation/PxSimpleTypes.h"
// #include "../foundation/PxVec3.h"
#include <PhysX/PxPhysics.h>

PhysicsMeshResource::PhysicsMeshResource() : Resource(),
                                             m_vertices(nullptr),
                                             m_indices(nullptr),
                                             m_numIndices(),
                                             m_numVertices() {
}

Resource::Status PhysicsMeshResource::fetchData(ResourceManager &resourceManager) {
    addDependency("physics");
    resourceManager.request(m_PhysicsResource, "physics");

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, m_Path);
    if (!warn.empty()) {
        Log::error(warn + " " + m_Path);

        return STATUS_FETCH_ERROR;
    }

    if (!err.empty()) {
        Log::error(err + " " + m_Path);

        return STATUS_FETCH_ERROR;
    }

    if (!res) {
        Log::error("Failed to load model: " + m_Path);

        return STATUS_FETCH_ERROR;
    }

    bool isSomeMeshLoaded = false;
    for (const auto &node: model.nodes) {
        if (node.mesh >= 0) {
            for (const auto &primitive: model.meshes[node.mesh].primitives) {
                if (primitive.indices < 0 || primitive.mode != TINYGLTF_MODE_TRIANGLES) {
                    continue;
                }
                tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
                m_numIndices = (int) indexAccessor.count;
                m_indices = new physx::PxU32[m_numIndices];

                const tinygltf::BufferView &bufferView = model.bufferViews[indexAccessor.bufferView];
                auto idx = (short *) (&model.buffers[bufferView.buffer].data.at(0) + bufferView.byteOffset);
                for (int i = 0; i < m_numIndices; ++i) {
                    m_indices[i] = (int) idx[i];
                }

                for (auto &attrib: primitive.attributes) {
                    if (attrib.first == "POSITION") {
                        tinygltf::Accessor vertexAccessor = model.accessors[attrib.second];
                        m_numVertices = (int) vertexAccessor.count;
                        m_vertices = new physx::PxVec3[m_numVertices];

                        const tinygltf::BufferView &vertexBufferView = model.bufferViews[vertexAccessor.bufferView];
                        auto vts = (physx::PxVec3 *) (&model.buffers[vertexBufferView.buffer].data.at(0) + vertexAccessor.byteOffset + vertexBufferView.byteOffset);

                        for (int i = 0; i < m_numVertices; ++i) {
                            m_vertices[i] = vts[i];
                        }

                        break;
                    }
                }

                if (m_vertices != nullptr) {
                    isSomeMeshLoaded = true;
                }
            }
        }
    }

    return isSomeMeshLoaded ? STATUS_DATA_READY : STATUS_FETCH_ERROR;
}

Resource::Status PhysicsMeshResource::build() {
    return STATUS_READY;
}

void PhysicsMeshResource::destroy() {
}
