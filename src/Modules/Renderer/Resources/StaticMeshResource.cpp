#include "StaticMeshResource.h"
// //#include "../../../SourceLibs/tinygltf/tiny_gltf.h"
// #include "../../../../libs/tinygltf/tiny_gltf.h"
// #include <GL/glew.h>
// #include <iostream>

// #define BUFFER_OFFSET(i) ((char *) NULL + (i))

StaticMeshResource::StaticMeshResource() {
}

Resource::Status StaticMeshResource::build() {
    //
    // tinygltf::Model model;
    // tinygltf::TinyGLTF loader;
    // std::string err;
    // std::string warn;
    //
    // bool res = loader.LoadASCIIFromFile(&model, &err, &warn, m_Path);
    // if (!warn.empty()) {
    //     std::cout << "WARN: " << warn << std::endl;
    // }
    //
    // if (!err.empty()) {
    //     std::cout << "ERR: " << err << std::endl;
    // }
    //
    // if (!res) {
    //     std::cout << "Failed to load model: " << m_Path << std::endl;
    // }
    //
    // glGenVertexArrays(1, &m_VertexArray.vertexArrayObjectId);
    // glBindVertexArray(m_VertexArray.vertexArrayObjectId);
    //
    // std::map<int, GLuint> generatedBuffers;
    // for (size_t i = 0; i < model.bufferViews.size(); ++i) {
    //     const tinygltf::BufferView &bufferView = model.bufferViews[i];
    //     const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
    //
    //     GLuint vbo;
    //     glGenBuffers(1, &vbo);
    //     generatedBuffers[i] = vbo;
    //     glBindBuffer(bufferView.target, vbo);
    //     glBufferData(bufferView.target, bufferView.byteLength,
    //                  &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
    //
    //     //        std::cout<<"B: "<<bufferView.target<<std::endl;
    // }
    //
    // for (const auto &node: model.nodes) {
    //     if (node.mesh >= 0) {
    //         for (const auto &primitive: model.meshes[node.mesh].primitives) {
    //             if (primitive.indices < 0) {
    //                 continue;
    //             }
    //
    //             tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
    //
    //             auto el = new VertexArray::Element();// el{};
    //             el->mode = primitive.mode;
    //             el->count = indexAccessor.count;
    //             el->componentType = indexAccessor.componentType;
    //             el->bufferOffset = indexAccessor.byteOffset;
    //             el->bufferId = generatedBuffers[indexAccessor.bufferView];
    //             m_VertexArray.elementsArray.push_back(el);
    //
    //             //                std::cout<<"Prim:"<<model.meshes[node.mesh].name<<std::endl;
    //
    //             for (auto &attrib: primitive.attributes) {
    //                 tinygltf::Accessor accessor = model.accessors[attrib.second];
    //
    //                 int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
    //                 glBindBuffer(GL_ARRAY_BUFFER, generatedBuffers[accessor.bufferView]);
    //
    //                 int size = 1;
    //                 if (accessor.type != TINYGLTF_TYPE_SCALAR) {
    //                     size = accessor.type;
    //                 }
    //
    //                 int vaa = -1;
    //                 if (attrib.first.compare("POSITION") == 0)
    //                     vaa = 0;
    //                 if (attrib.first.compare("NORMAL") == 0)
    //                     vaa = 1;
    //                 if (attrib.first.compare("TEXCOORD_0") == 0)
    //                     vaa = 2;
    //                 if (attrib.first.compare("TANGENT") == 0)
    //                     vaa = 3;
    //                 if (vaa > -1) {
    //
    //                     //                        std::cout<<"VA: "<<vaa<<", "<<size<<", "<<accessor.componentType<<std::endl;
    //                     glEnableVertexAttribArray(vaa);
    //                     glVertexAttribPointer(vaa, size, accessor.componentType,
    //                                           accessor.normalized ? GL_TRUE : GL_FALSE,
    //                                           byteStride, BUFFER_OFFSET(accessor.byteOffset));
    //                 }
    //             }
    //         }
    //     }
    // }
    //
    // glBindVertexArray(0);

    return STATUS_READY;
}
void StaticMeshResource::render() {
    // m_VertexArray.draw();
}
