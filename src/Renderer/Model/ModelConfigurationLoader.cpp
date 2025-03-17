#include "ModelConfigurationLoader.h"
#include "../../Core/Helper/Log.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

ModelConfigurationLoader::ModelConfigurationLoader()
= default;

void ModelConfigurationLoader::createFromGLTFModel(tinygltf::Model &modelIn, ModelConfiguration &modelOut) {
    modelOut.indices.clear();
    modelOut.vertices.clear();
    modelOut.nodes.clear();

    modelOut.nodes.reserve(modelIn.meshes.size());

    for (const auto &scene: modelIn.scenes) {
        for (const auto &node: scene.nodes) {
            auto modelMatrix = glm::mat4(1.0f);
            loadNode(modelIn, modelIn.nodes[node], modelMatrix, modelOut);
        }
    }
}

void ModelConfigurationLoader::loadNode(tinygltf::Model &model,
                                        const tinygltf::Node &node,
                                        const glm::mat4 &parentTransform,
                                        ModelConfiguration &configuration) {
    glm::mat4 transform = getNodeTransform(node) * parentTransform;

    if (node.mesh > -1) {
        assert(node.mesh < model.meshes.size());

        if (const auto mesh = model.meshes[node.mesh]; !mesh.primitives.empty()) {
            auto &meshNode = configuration.nodes.emplace_back();
            meshNode.name = node.name;
            meshNode.modelMatrix = transform;

            loadMesh(model, mesh, transform, configuration, meshNode);
        }
    }

    for (int i: node.children) {
        assert(i < model.nodes.size());
        loadNode(model, model.nodes[i], transform, configuration);
    }
}

void ModelConfigurationLoader::loadMesh(const tinygltf::Model &model,
                                        const tinygltf::Mesh &mesh,
                                        glm::mat4 &transform,
                                        ModelConfiguration &configuration,
                                        Model::MeshNode &meshNode) {
    meshNode.offset = static_cast<int>(configuration.indices.size());
    const unsigned long vertexOffset = configuration.vertices.size();
    unsigned long indicesSize = 0;

    for (const auto &primitive: mesh.primitives) {
        if (primitive.indices < 0 || primitive.mode != TINYGLTF_MODE_TRIANGLES) {
            continue;
        }

        if (primitive.material >= 0) {
            meshNode.materialIndex = primitive.material;
        }

        const tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
        const unsigned long numIndices = indexAccessor.count;

        const tinygltf::BufferView &indexBufferView = model.bufferViews[indexAccessor.bufferView];
        const unsigned char *indexDataPtr = model.buffers[indexBufferView.buffer].data.data() + indexBufferView.
                                            byteOffset + indexAccessor.byteOffset;

        configuration.indices.reserve(configuration.indices.size() + numIndices);
        for (int i = 0; i < numIndices; ++i) {
            const uint32_t index = *(reinterpret_cast<const uint16_t *>(indexDataPtr) + i);
            configuration.indices.push_back(index + vertexOffset);
        }
        indicesSize += numIndices;

        const int positionAttributeIndex = getPrimitiveAttributeIndex(primitive, POSITION_ATTRIB_NAME);
        if (positionAttributeIndex < 0) {
            continue;
        }

        const auto numVertices = model.accessors[positionAttributeIndex].count;
        const auto verticesData = getBufferData<glm::vec3>(model, primitive, POSITION_ATTRIB_NAME);
        const auto texCoordsData = getBufferData<glm::vec2>(model, primitive, TEXCOORD_ATTRIB_NAME);
        const auto normalsData = getBufferData<glm::vec3>(model, primitive, NORMAL_ATTRIB_NAME);

        configuration.vertices.reserve(configuration.vertices.size() + numVertices);
        for (int i = 0; i < numVertices; ++i) {
            VertexArray::Vertex vertex{};

            vertex.position = verticesData[i];
            if (texCoordsData != nullptr) {
                vertex.texCoord = texCoordsData[i];
            }

            if (normalsData != nullptr) {
                vertex.normal = normalsData[i];
            }

            configuration.vertices.push_back(vertex);
        }
    }

    meshNode.size += static_cast<int>(indicesSize);
}

int ModelConfigurationLoader::getPrimitiveAttributeIndex(const tinygltf::Primitive &primitive,
                                                         const std::string &attributeName) {
    for (auto &attrib: primitive.attributes) {
        if (attrib.first == attributeName && attrib.second >= 0) {
            return attrib.second;
        }
    }

    return -1;
}

glm::mat4 ModelConfigurationLoader::getNodeTransform(const tinygltf::Node &node) {
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

glm::mat4 ModelConfigurationLoader::convertDoubleArrayToGlmMat4(const double arr[16]) {
    glm::mat4 glmMatrix;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            glmMatrix[col][row] = static_cast<float>(arr[col * 4 + row]);
        }
    }

    return glmMatrix;
}
