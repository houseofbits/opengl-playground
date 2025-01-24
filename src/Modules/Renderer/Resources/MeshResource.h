#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Model/VertexArray.h"
#include "../../../../libs/tinygltf/tiny_gltf.h"
#include <GL/glew.h>
#include "ShaderProgramResource.h"

class MeshResource : public Resource {
public:
    MeshResource();

    class MeshInstance {
    public:
        std::string name{};
        //material settings
        glm::mat4 modelMatrix{1.0};
        VertexArray vertexArray{};
    };

    Resource::Status build() override;

    void destroy() override {
        //TODO Unload the mesh resource
    }

    void render(ShaderProgramResource &) const;

private:
    void loadBufferData(tinygltf::Model &model);

    void loadNode(tinygltf::Model &model, const tinygltf::Node &node, const glm::mat4 &parentTransform);

    void loadMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh, glm::mat4 &transform);

    MeshInstance *createMeshInstance(std::string name, glm::mat4 modelMatrix);

    VertexArray::Element *createPrimitive(
        const tinygltf::Accessor &indexAccessor,
        int primitiveGLMode,
        int vertexArrayObjectId
    );

    static size_t componentTypeByteSize(int type);

    static glm::mat4 getNodeTransform(const tinygltf::Node &node);

    static int getGLPrimitiveMode(int gltfMode);

    static int getTypeSize(int gltfType);

    static int getVertexAttributeIndex(const std::string &attributeName);

    static glm::mat4 convertDoubleArrayToGlmMat4(const double arr[16]);

    std::vector<MeshInstance *> m_meshes{};
    std::map<unsigned int, GLuint> m_vertexBuffers{};
};
