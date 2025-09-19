#define GLM_ENABLE_EXPERIMENTAL

#include "ModelConfigurationLoader.h"
#include "../../Core/Helper/Log.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

ModelConfigurationLoader::ModelConfigurationLoader()
= default;

void ModelConfigurationLoader::createFromGLTFModel(tinygltf::Model &modelIn, ModelConfiguration &modelOut) {
    createFromGLTFModel(modelIn, modelOut, {});
}

void ModelConfigurationLoader::createFromGLTFModel(tinygltf::Model &modelIn, ModelConfiguration &modelOut,
                                                   LoaderConfiguration configuration) {
    modelOut.indices.clear();
    modelOut.vertices.clear();
    modelOut.nodes.clear();

    modelOut.nodes.reserve(modelIn.meshes.size());

    for (const auto &scene: modelIn.scenes) {
        for (const auto &node: scene.nodes) {
            auto modelMatrix = glm::mat4(1.0f);
            loadNode(modelIn, modelIn.nodes[node], modelMatrix, modelOut, configuration);
        }
    }

    if (configuration.generateTangents) {
        generateTangents(modelOut);
    }
}

void ModelConfigurationLoader::loadNode(tinygltf::Model &model,
                                        const tinygltf::Node &node,
                                        const glm::mat4 &parentTransform,
                                        ModelConfiguration &modelOut,
                                        const LoaderConfiguration &configuration) {
    glm::mat4 transform = getNodeTransform(node) * parentTransform;

    if (node.mesh > -1) {
        assert(node.mesh < model.meshes.size());

        if (const auto mesh = model.meshes[node.mesh]; !mesh.primitives.empty()) {
            auto &meshNode = modelOut.nodes.emplace_back();
            meshNode.name = node.name;
            meshNode.modelMatrix = transform;
            meshNode.scaling = getNodeScaling(node);

            if (configuration.bakeTransforms) {
                meshNode.modelMatrix = glm::mat4(1.0f);
            }

            loadMesh(model, mesh, transform, modelOut, meshNode, configuration);
        }
    }

    for (int i: node.children) {
        assert(i < model.nodes.size());
        loadNode(model, model.nodes[i], transform, modelOut, configuration);
    }
}

void ModelConfigurationLoader::loadMesh(const tinygltf::Model &model,
                                        const tinygltf::Mesh &mesh,
                                        glm::mat4 &transform,
                                        ModelConfiguration &modelOut,
                                        Model::MeshNode &meshNode,
                                        const LoaderConfiguration &configuration) {
    meshNode.offset = static_cast<int>(modelOut.indices.size());
    const unsigned long vertexOffset = modelOut.vertices.size();
    unsigned long indicesSize = 0;


    // glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(transform)));
    // auto isInverted = false;
    if (meshNode.name == "Plane140.006") {
        // glm::vec3 scale;
        //
        // auto col0 = glm::vec3(transform[0]);
        // auto col1 = glm::vec3(transform[1]);
        // auto col2 = glm::vec3(transform[2]);
        //
        // scale.x = glm::length(col0);
        // scale.y = glm::length(col1);
        // scale.z = glm::length(col2);
        //
        // // Prevent divide-by-zero
        // if (scale.x == 0.0f) scale.x = 1.0f;
        // if (scale.y == 0.0f) scale.y = 1.0f;
        // if (scale.z == 0.0f) scale.z = 1.0f;
        //
        // glm::mat3 rotMat;
        // rotMat[0] = col0 / scale.x;
        // rotMat[1] = col1 / scale.y;
        // rotMat[2] = col2 / scale.z;
        //
        // if (glm::determinant(rotMat) < 0.0f) {
        //     // Log::write("Is inverted");
        //     isInverted = true;
        //     // Flip one axis (choose X by convention)
        //     // scale.x = -scale.x;
        //     // rotMat[0] = -rotMat[0];
        // }

        // glm::vec3 scale;
        // scale.x = glm::length(glm::vec3(transform[0]));
        // scale.y = glm::length(glm::vec3(transform[1]));
        // scale.z = glm::length(glm::vec3(transform[2]));
        //
        // Log::write(meshNode.name, " scale ", meshNode.scaling);
    }

    for (const auto &primitive: mesh.primitives) {
        if (primitive.indices < 0 || primitive.mode != TINYGLTF_MODE_TRIANGLES) {
            Log::warn("Mesh is not triangulated ", meshNode.name);
            continue;
        }

        if (primitive.material >= 0) {
            meshNode.materialIndex = primitive.material;
        }

        indicesSize += loadMeshIndices(model, primitive.indices, vertexOffset, modelOut);

        const int positionAttributeIndex = getPrimitiveAttributeIndex(primitive, POSITION_ATTRIB_NAME);
        if (positionAttributeIndex < 0) {
            continue;
        }

        const auto numVertices = model.accessors[positionAttributeIndex].count;
        const auto verticesData = getBufferData<glm::vec3>(model, primitive, POSITION_ATTRIB_NAME);
        const auto texCoordsData = getBufferData<glm::vec2>(model, primitive, TEXCOORD_ATTRIB_NAME);
        const auto normalsData = getBufferData<glm::vec3>(model, primitive, NORMAL_ATTRIB_NAME);
        const auto tangentsData = getBufferData<glm::vec3>(model, primitive, TANGENT_ATTRIB_NAME);

        if (texCoordsData == nullptr) {
            modelOut.hasTexCoords = false;
        }

        if (tangentsData == nullptr) {
            modelOut.hasTangents = false;
        }

        if (normalsData == nullptr) {
            modelOut.hasNormals = false;
        }

        modelOut.vertices.reserve(modelOut.vertices.size() + numVertices);
        for (int i = 0; i < numVertices; ++i) {
            VertexArray::Vertex vertex{};

            vertex.position = verticesData[i];

            if (configuration.bakeTransforms) {
                vertex.position = transform * glm::vec4(vertex.position, 1.0);
            }

            if (texCoordsData != nullptr) {
                vertex.texCoord = texCoordsData[i];
            }

            if (normalsData != nullptr) {
                // auto m = transform;
                // m[3] = glm::vec4(0.0f, 0.0f, 0.0f, m[3].w);

                // glm::mat4 m = glm::mat4(1.0);
                //
                // glm::vec3 scale;
                // scale.x = glm::length(glm::vec3(transform[0]));
                // scale.y = glm::length(glm::vec3(transform[1]));
                // scale.z = glm::length(glm::vec3(transform[2]));
                //
                // glm::mat3 rotation;
                // rotation[0] = glm::vec3(m[0]) / scale.x;
                // rotation[1] = glm::vec3(m[1]) / scale.y;
                // rotation[2] = glm::vec3(m[2]) / scale.z;

                auto v = normalsData[i];// * meshNode.scaling;

                // if (isInverted) {
                //     v.y = -v.y;
                // }

                // v = glm::normalize(v * normalMatrix);

                vertex.normal = glm::normalize(v); //glm::inverse(m) * glm::vec4(normalsData[i], 1.0);
            }

            if (tangentsData != nullptr) {
                vertex.tangent = tangentsData[i];
            }

            modelOut.vertices.push_back(vertex);
        }
    }

    meshNode.size += static_cast<int>(indicesSize);
}

int ModelConfigurationLoader::loadMeshIndices(const tinygltf::Model &model,
                                              const int indicesAccessor,
                                              const int vertexOffset,
                                              ModelConfiguration &configuration) {
    const tinygltf::Accessor indexAccessor = model.accessors[indicesAccessor];
    const unsigned long numIndices = indexAccessor.count;

    const tinygltf::BufferView &indexBufferView = model.bufferViews[indexAccessor.bufferView];
    const unsigned char *indexDataPtr = model.buffers[indexBufferView.buffer].data.data() + indexBufferView.
                                        byteOffset + indexAccessor.byteOffset;

    configuration.indices.reserve(configuration.indices.size() + numIndices);
    for (int i = 0; i < numIndices; ++i) {
        const uint32_t index = *(reinterpret_cast<const uint16_t *>(indexDataPtr) + i);
        configuration.indices.push_back(index + vertexOffset);
    }

    return numIndices;
}

void ModelConfigurationLoader::generateTangents(ModelConfiguration &configuration) {
    for (size_t i = 0; i < configuration.indices.size(); i += 3) {
        unsigned int i0 = configuration.indices[i];
        unsigned int i1 = configuration.indices[i + 1];
        unsigned int i2 = configuration.indices[i + 2];

        auto &v0 = configuration.vertices[i0];
        auto &v1 = configuration.vertices[i1];
        auto &v2 = configuration.vertices[i2];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;

        glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
        glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        glm::vec3 biTangent;
        biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        biTangent = glm::normalize(biTangent);

        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;

        v0.biTangent += biTangent;
        v1.biTangent += biTangent;
        v2.biTangent += biTangent;
    }

    for (auto &vertex: configuration.vertices) {
        vertex.tangent = glm::normalize(vertex.tangent);
        vertex.biTangent = glm::normalize(vertex.biTangent);
    }
}

void ModelConfigurationLoader::bakeNodes(ModelConfiguration &configuration) {
    for (const auto &node: configuration.nodes) {
        for (int i = 0; i < node.size; ++i) {
        }
    }
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

glm::vec3 ModelConfigurationLoader::getNodeScaling(const tinygltf::Node &node) {
    if (node.scale.size() == 3) {
        return {
            node.scale[0],
            node.scale[1],
            node.scale[2]
        };
    }

    return glm::vec3(1.0);
}

glm::mat4 ModelConfigurationLoader::getNodeTransform(const tinygltf::Node &node) {
    //
    // // if (node.name == "Plane140.006") {
    // //     Log::write(node.scale.size(), " ", node.scale[0], ", ", node.scale[1], ", ", node.scale[2]);
    // // }
    //
    // if (node.matrix.size() == 16) {
    //     return convertDoubleArrayToGlmMat4(node.matrix.data());
    // }
    //
    // auto transform = glm::mat4(1.0f);
    // if (node.translation.size() == 3) {
    //     transform = glm::translate(transform, {
    //                                    node.translation[0], node.translation[1], node.translation[2]
    //                                });
    // }
    // if (node.rotation.size() == 4) {
    //     const auto rotation = glm::quat(static_cast<float>(node.rotation[3]),
    //                                     static_cast<float>(node.rotation[0]),
    //                                     static_cast<float>(node.rotation[1]),
    //                                     static_cast<float>(node.rotation[2])
    //     );
    //     transform *= glm::mat4_cast(rotation);
    // }
    // if (node.scale.size() == 3) {
    //     transform = glm::scale(transform, {
    //                                node.scale[0], node.scale[1], node.scale[2]
    //                            });
    // }
    //
    // // if (node.name == "Plane140.006") {
    // //     glm::vec3 scale;
    // //     scale.x = glm::length(glm::vec3(transform[0]));
    // //     scale.y = glm::length(glm::vec3(transform[1]));
    // //     scale.z = glm::length(glm::vec3(transform[2]));
    // //
    // //     Log::write("Scale ", scale);
    // // }
    //
    // return transform;

    glm::mat4 matrix(1.0f);

    if (node.matrix.size() == 16) {
        // Directly use provided 4x4 matrix
        matrix = glm::make_mat4(node.matrix.data());
    } else {
        // Build from T * R * S
        glm::vec3 translation(0.0f);
        glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f); // w, x, y, z
        glm::vec3 scale(1.0f);

        if (node.translation.size() == 3) {
            translation = glm::vec3(
                node.translation[0],
                node.translation[1],
                node.translation[2]
            );
        }

        if (node.rotation.size() == 4) {
            rotation = glm::quat(
                static_cast<float>(node.rotation[3]), // w
                static_cast<float>(node.rotation[0]), // x
                static_cast<float>(node.rotation[1]), // y
                static_cast<float>(node.rotation[2]) // z
            );
        }

        if (node.scale.size() == 3) {
            scale = glm::vec3(
                node.scale[0],
                node.scale[1],
                node.scale[2]
            );
        }

        const glm::mat4 T = glm::translate(glm::mat4(1.0f), translation);
        const glm::mat4 R = glm::toMat4(rotation);
        const glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

        matrix = T * R * S;
    }

    return matrix;
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
