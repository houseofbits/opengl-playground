#pragma once
#include "Model.h"
#include "../../../libs/tinygltf/tiny_gltf.h"
#include <GL/glew.h>

class ModelGLTFLoader {
public:
    ModelGLTFLoader();

    static void createFromGLTFModel(tinygltf::Model &modelIn, Model &modelOut);

private:
    typedef std::map<unsigned int, GLuint> BufferMap;

    static BufferMap loadBuffers(const tinygltf::Model &modelIn);

    static void loadNode(tinygltf::Model &model,
                         const tinygltf::Node &node,
                         const glm::mat4 &parentTransform,
                         const BufferMap &vertexBuffers,
                         Model &modelOut);

    static void loadMesh(const tinygltf::Model &model,
                         const tinygltf::Mesh &mesh,
                         glm::mat4 &transform,
                         const BufferMap &vertexBuffers,
                         Model::MeshInstance &instanceOut);

    static void loadMeshWithRebuild(const tinygltf::Model &model,
                                    const tinygltf::Mesh &mesh,
                                    glm::mat4 &transform,
                                    Model::MeshInstance &instanceOut);

    static int getPrimitiveAttributeIndex(const tinygltf::Primitive &primitive, const std::string &attributeName);

    template<class T>
    static T *getBufferData(const tinygltf::Model &model, const tinygltf::Primitive &primitive,
                            const std::string &attributeName) {
        const int index = getPrimitiveAttributeIndex(primitive, attributeName);
        if (index >= 0) {
            const tinygltf::Accessor accessor = model.accessors[index];

            const tinygltf::BufferView &vertexBufferView = model.bufferViews[accessor.bufferView];
            return (T *)(&model.buffers[vertexBufferView.buffer].data.at(0) +
                                    accessor.byteOffset + vertexBufferView.byteOffset);
        }

        return nullptr;
    }

    static glm::mat4 getNodeTransform(const tinygltf::Node &node);

    static glm::mat4 convertDoubleArrayToGlmMat4(const double arr[16]);

    static size_t componentTypeByteSize(int type);

    static int getGLPrimitiveMode(int gltfMode);

    static int getTypeSize(int gltfType);

    static int getVertexAttributeIndex(const std::string &attributeName);

    static VertexArray::Element *createPrimitive(
        const tinygltf::Accessor &indexAccessor,
        int primitiveGLMode,
        unsigned int vertexArrayObjectId,
        const BufferMap &vertexBuffers
    );
};
