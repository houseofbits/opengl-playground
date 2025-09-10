#pragma once

#include "Model.h"
#include "../../../libs/tinygltf/tiny_gltf.h"

class ModelConfigurationLoader {
    inline static const std::string POSITION_ATTRIB_NAME = "POSITION";
    inline static const std::string TEXCOORD_ATTRIB_NAME = "TEXCOORD_0";
    inline static const std::string NORMAL_ATTRIB_NAME = "NORMAL";
    inline static const std::string TANGENT_ATTRIB_NAME = "TANGENT";

public:
    class ModelConfiguration {
    public:
        std::vector<VertexArray::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Model::MeshNode> nodes;
        bool hasTexCoords{true};
        bool hasNormals{true};
        bool hasTangents{true};
    };

    class LoaderConfiguration {
    public:
        bool generateTangents{true};
        bool bakeTransforms{false};
    };

    ModelConfigurationLoader();

    static void createFromGLTFModel(tinygltf::Model &modelIn, ModelConfiguration &modelOut);

    static void createFromGLTFModel(tinygltf::Model &modelIn, ModelConfiguration &modelOut,
                                    LoaderConfiguration configuration);

private:
    static void loadNode(tinygltf::Model &model,
                         const tinygltf::Node &node,
                         const glm::mat4 &parentTransform,
                         ModelConfiguration &modelOut,
                         const LoaderConfiguration &configuration);

    static void loadMesh(const tinygltf::Model &model,
                         const tinygltf::Mesh &mesh,
                         glm::mat4 &transform,
                         ModelConfiguration &modelOut,
                         Model::MeshNode &meshNode,
                         const LoaderConfiguration &configuration);

    static int loadMeshIndices(const tinygltf::Model &model,
                               int indicesAccessor,
                               int vertexOffset,
                               ModelConfiguration &configuration);

    static void bakeNodes(ModelConfiguration &configuration);

    static void generateTangents(ModelConfiguration &configuration);

    static int getPrimitiveAttributeIndex(const tinygltf::Primitive &primitive, const std::string &attributeName);

    template<class T>
    static T *getBufferData(const tinygltf::Model &model, const tinygltf::Primitive &primitive,
                            const std::string &attributeName) {
        if (const int index = getPrimitiveAttributeIndex(primitive, attributeName); index >= 0) {
            const tinygltf::Accessor accessor = model.accessors[index];

            const tinygltf::BufferView &vertexBufferView = model.bufferViews[accessor.bufferView];
            return (T *) (&model.buffers[vertexBufferView.buffer].data.at(0) +
                          accessor.byteOffset + vertexBufferView.byteOffset);
        }

        return nullptr;
    }

    static glm::mat4 getNodeTransform(const tinygltf::Node &node);

    static glm::vec3 getNodeScaling(const tinygltf::Node &node);

    static glm::mat4 convertDoubleArrayToGlmMat4(const double arr[16]);
};
