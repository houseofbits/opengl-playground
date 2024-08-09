#include "PhysicsMeshResource.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include "foundation/PxSimpleTypes.h"
#include "foundation/PxVec3.h"

PhysicsMeshResource::PhysicsMeshResource() : Resource(), m_pxTriangleMeshDescriptor() {
}

Resource::Status PhysicsMeshResource::fetchData(ResourceManager &) {
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

                physx::PxU32 *indices = nullptr;
                physx::PxVec3 *vertices = nullptr;
                int numIndices = (int) indexAccessor.count;
                int numVertices = 0;

                const tinygltf::BufferView &bufferView = model.bufferViews[indexAccessor.bufferView];
                indices = (physx::PxU32 *) (&model.buffers[bufferView.buffer].data.at(0) + bufferView.byteOffset);

                for (auto &attrib: primitive.attributes) {
                    if (attrib.first == "POSITION") {
                        tinygltf::Accessor vertexAccessor = model.accessors[attrib.second];
                        const tinygltf::BufferView &vertexBufferView = model.bufferViews[vertexAccessor.bufferView];
                        vertices = (physx::PxVec3 *) (&model.buffers[vertexBufferView.buffer].data.at(0) + vertexBufferView.byteOffset);
                        numVertices = (int) vertexAccessor.count;
                    }
                }

                if (indices && vertices) {
                    m_pxTriangleMeshDescriptor = createTriangleMeshDescriptor(numVertices, vertices, numIndices, indices);
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

physx::PxTriangleMeshDesc PhysicsMeshResource::createTriangleMeshDescriptor(int numVertices, physx::PxVec3 *vertices, int numIndices, const physx::PxU32 *indices) {
    physx::PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = numVertices;
    meshDesc.points.stride = sizeof(physx::PxVec3);
    meshDesc.points.data = vertices;

    meshDesc.triangles.count = numIndices / 3;
    meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
    meshDesc.triangles.data = indices;

    return meshDesc;
}
