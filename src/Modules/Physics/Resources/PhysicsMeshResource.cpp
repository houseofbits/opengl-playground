#include "PhysicsMeshResource.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"

PhysicsMeshResource::PhysicsMeshResource() : Resource() {
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

    for (const auto &node: model.nodes) {
        if (node.mesh >= 0) {
            for (const auto &primitive: model.meshes[node.mesh].primitives) {
                if (primitive.indices < 0) {
                    continue;
                }

                tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

//                std::cout<<indexAccessor.bufferView<<std::endl;

                const tinygltf::BufferView &bufferView = model.bufferViews[indexAccessor.bufferView];
                const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

//                std::cout<<(primitive.mode == TINYGLTF_MODE_TRIANGLES)<<std::endl;
                std::cout<<indexAccessor.count<<std::endl;
                auto * data = &buffer.data.at(0) + bufferView.byteOffset;
                for (int i = 0; i < indexAccessor.count; ++i) {
//                    std::cout<<i<<" "<<(int)data[i]<<std::endl;
                }


                //                VertexArray::Element el{};
//                el.mode = primitive.mode;
//                el.count = indexAccessor.count;
//                el.componentType = indexAccessor.componentType;
//                el.bufferOffset = indexAccessor.byteOffset;
//                el.bufferId = generatedBuffers[indexAccessor.bufferView];
            }
        }
    }

    return STATUS_DATA_READY;
}

Resource::Status PhysicsMeshResource::build() {
    return STATUS_READY;
}

void PhysicsMeshResource::destroy() {
}
