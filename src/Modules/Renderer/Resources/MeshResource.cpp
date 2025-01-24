#include "MeshResource.h"
#include <iostream>
#include <string>
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

MeshResource::MeshResource() {
}

Resource::Status MeshResource::build() {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, m_Path);
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res) {
        std::cout << "Failed to load model: " << m_Path << std::endl;
    }

    loadBufferData(model);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    loadNode(model, model.nodes[0], modelMatrix);

    return STATUS_READY;
}

void MeshResource::loadBufferData(tinygltf::Model &model) {
    for (size_t i = 0; i < model.bufferViews.size(); i++) {
        const tinygltf::BufferView &bufferView = model.bufferViews[i];
        if (bufferView.target == 0) {
            Log::warn("bufferView.target is zero");

            continue;
        }

        int sparse_accessor = -1;
        for (size_t a_i = 0; a_i < model.accessors.size(); ++a_i) {
            const auto &accessor = model.accessors[a_i];
            if (accessor.bufferView == i) {
                if (accessor.sparse.isSparse) {
                    Log::warn(
                        "WARN: this bufferView has at least one sparse accessor to it. We are going to load the data as patched by this sparse accessor, not the original data");
                    sparse_accessor = static_cast<int>(a_i);

                    break;
                }
            }
        }

        const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
        GLuint vbId;
        glGenBuffers(1, &vbId);
        glBindBuffer(bufferView.target, vbId);

        if (sparse_accessor < 0) {
            glBufferData(bufferView.target, static_cast<GLsizeiptr>(bufferView.byteLength),
                         &buffer.data.at(0) + bufferView.byteOffset,
                         GL_STATIC_DRAW);
        } else {
            const auto accessor = model.accessors[sparse_accessor];
            auto *tmp_buffer = new unsigned char[bufferView.byteLength];
            memcpy(tmp_buffer, buffer.data.data() + bufferView.byteOffset,
                   bufferView.byteLength);

            const size_t size_of_object_in_buffer =
                    componentTypeByteSize(accessor.componentType);
            const size_t size_of_sparse_indices =
                    componentTypeByteSize(accessor.sparse.indices.componentType);

            const auto &indices_buffer_view =
                    model.bufferViews[accessor.sparse.indices.bufferView];
            const auto &indices_buffer = model.buffers[indices_buffer_view.buffer];

            const auto &values_buffer_view =
                    model.bufferViews[accessor.sparse.values.bufferView];
            const auto &values_buffer = model.buffers[values_buffer_view.buffer];

            for (size_t sparse_index = 0; sparse_index < accessor.sparse.count; ++sparse_index) {
                int index = 0;
                switch (accessor.sparse.indices.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_BYTE:
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                        index = static_cast<int>(*const_cast<unsigned char *>(indices_buffer.data.data() +
                                                                              indices_buffer_view.byteOffset +
                                                                              accessor.sparse.indices.byteOffset +
                                                                              (sparse_index * size_of_sparse_indices)));
                        break;
                    case TINYGLTF_COMPONENT_TYPE_SHORT:
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                        index = static_cast<int>(*(
                            unsigned short *) (indices_buffer.data.data() +
                                               indices_buffer_view.byteOffset +
                                               accessor.sparse.indices.byteOffset +
                                               (sparse_index * size_of_sparse_indices)));
                        break;
                    case TINYGLTF_COMPONENT_TYPE_INT:
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                        index = static_cast<int>(*(
                            unsigned int *) (indices_buffer.data.data() +
                                             indices_buffer_view.byteOffset +
                                             accessor.sparse.indices.byteOffset +
                                             (sparse_index * size_of_sparse_indices)));
                        break;
                    default: continue;
                }

                const unsigned char *read_from =
                        values_buffer.data.data() +
                        (values_buffer_view.byteOffset +
                         accessor.sparse.values.byteOffset) +
                        (sparse_index * (size_of_object_in_buffer * accessor.type));

                unsigned char *write_to =
                        tmp_buffer + index * (size_of_object_in_buffer * accessor.type);

                memcpy(write_to, read_from, size_of_object_in_buffer * accessor.type);

                glBufferData(bufferView.target, static_cast<GLsizeiptr>(bufferView.byteLength), tmp_buffer,
                             GL_STATIC_DRAW);
                delete[] tmp_buffer;
            }
        }

        glBindBuffer(bufferView.target, 0);

        m_vertexBuffers[i] = vbId;
    }
}

void MeshResource::loadNode(tinygltf::Model &model, const tinygltf::Node &node, const glm::mat4 &parentTransform) {
    glm::mat4 transform = getNodeTransform(node) * parentTransform;

    if (node.mesh > -1) {
        assert(node.mesh < model.meshes.size());
        loadMesh(model, model.meshes[node.mesh], transform);
    }

    for (int i: node.children) {
        assert(i < model.nodes.size());
        loadNode(model, model.nodes[i], transform);
    }
}

void MeshResource::loadMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh, glm::mat4 &transform) {
    auto meshInstance = createMeshInstance(mesh.name, transform);

    for (const auto &primitive: mesh.primitives) {
        if (primitive.indices < 0) continue;

        unsigned int vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        auto it(primitive.attributes.begin());
        for (auto itEnd(primitive.attributes.end()); it != itEnd; ++it) {
            assert(it->second >= 0);
            const tinygltf::Accessor &accessor = model.accessors[it->second];

            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[accessor.bufferView]);

            if (int vaa = getVertexAttributeIndex(it->first); vaa > -1) {
                const int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                assert(byteStride != -1);

                glVertexAttribPointer(vaa,
                                      getTypeSize(accessor.type),
                                      accessor.componentType,
                                      accessor.normalized ? GL_TRUE : GL_FALSE,
                                      byteStride, BUFFER_OFFSET(accessor.byteOffset));
                glEnableVertexAttribArray(vaa);
            }
        }

        const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];

        meshInstance->vertexArray.elementsArray.push_back(
            createPrimitive(indexAccessor, getGLPrimitiveMode(primitive.mode), vao));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBuffers[indexAccessor.bufferView]);
    }

    glBindVertexArray(0);
}

void MeshResource::render(ShaderProgramResource &shader) const {
    for (auto &mesh: m_meshes) {
        shader.setUniform("modelMatrix", mesh->modelMatrix);
        for (const auto &element: mesh->vertexArray.elementsArray) {
            glBindVertexArray(element->vaoId);
            glDrawElements(element->mode, static_cast<GLsizei>(element->count),
                           element->componentType,
                           BUFFER_OFFSET(element->bufferOffset));
        }
    }
}

MeshResource::MeshInstance *MeshResource::createMeshInstance(std::string name, glm::mat4 transform) {
    m_meshes.push_back(new MeshInstance());
    auto meshInstance = m_meshes.back();
    meshInstance->name = name;
    meshInstance->modelMatrix = transform;

    return meshInstance;
}

VertexArray::Element *MeshResource::createPrimitive(
    const tinygltf::Accessor &indexAccessor,
    int primitiveGLMode,
    int vertexArrayObjectId
) {
    auto el = new VertexArray::Element();
    el->mode = primitiveGLMode;
    el->count = indexAccessor.count;
    el->componentType = indexAccessor.componentType;
    el->bufferOffset = indexAccessor.byteOffset;
    el->bufferId = m_vertexBuffers[indexAccessor.bufferView];
    el->vaoId = vertexArrayObjectId;

    return el;
}

glm::mat4 MeshResource::getNodeTransform(const tinygltf::Node &node) {
    if (node.matrix.size() == 16) {
        return convertDoubleArrayToGlmMat4(node.matrix.data());
    }

    auto transform = glm::mat4(1.0f);
    if (node.translation.size() == 3) {
        transform = glm::translate(transform, {
                                       node.translation[0], node.translation[1], node.translation[2]
                                   });
    }
    if (node.rotation.size() == 4) {
        const auto rotation = glm::quat(static_cast<float>(node.rotation[3]),
                                        static_cast<float>(node.rotation[0]),
                                        static_cast<float>(node.rotation[1]),
                                        static_cast<float>(node.rotation[2])
        );
        transform *= glm::mat4_cast(rotation);
    }
    if (node.scale.size() == 3) {
        transform = glm::scale(transform, {
                                   node.scale[0], node.scale[1], node.scale[2]
                               });
    }
    return transform;
}

glm::mat4 MeshResource::convertDoubleArrayToGlmMat4(const double arr[16]) {
    glm::mat4 glmMatrix;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            glmMatrix[col][row] = static_cast<float>(arr[col * 4 + row]);
        }
    }

    return glmMatrix;
}

int MeshResource::getGLPrimitiveMode(const int gltfMode) {
    switch (gltfMode) {
        case TINYGLTF_MODE_TRIANGLES: return GL_TRIANGLES;
        case TINYGLTF_MODE_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        case TINYGLTF_MODE_TRIANGLE_FAN: return GL_TRIANGLE_FAN;
        case TINYGLTF_MODE_POINTS: return GL_POINTS;
        case TINYGLTF_MODE_LINE: return GL_LINES;
        case TINYGLTF_MODE_LINE_LOOP: return GL_LINE_LOOP;
        default: assert(0);
    }
}

int MeshResource::getTypeSize(int gltfType) {
    switch (gltfType) {
        case TINYGLTF_TYPE_SCALAR: return 1;
        case TINYGLTF_TYPE_VEC2: return 2;
        case TINYGLTF_TYPE_VEC3: return 3;
        case TINYGLTF_TYPE_VEC4: return 4;
        default: assert(0);
    }
}

int MeshResource::getVertexAttributeIndex(const std::string &attributeName) {
    if (attributeName == "POSITION")
        return 0;
    if (attributeName == "NORMAL")
        return 1;
    if (attributeName == "TEXCOORD_0")
        return 2;
    if (attributeName == "TANGENT")
        return 3;

    return -1;
}

size_t MeshResource::componentTypeByteSize(const int type) {
    switch (type) {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        case TINYGLTF_COMPONENT_TYPE_BYTE:
            return sizeof(char);
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        case TINYGLTF_COMPONENT_TYPE_SHORT:
            return sizeof(short);
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        case TINYGLTF_COMPONENT_TYPE_INT:
            return sizeof(int);
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            return sizeof(float);
        case TINYGLTF_COMPONENT_TYPE_DOUBLE:
            return sizeof(double);
        default:
            return 0;
    }
}
