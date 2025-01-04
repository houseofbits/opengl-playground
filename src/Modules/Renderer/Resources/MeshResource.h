#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Model/VertexArray.h"
#include <fbxsdk.h>

class MeshResource : public Resource {
public:
    MeshResource();

    class MeshInstance {
    public:
        std::string name;
        //material settings
        glm::mat4 modelMatrix;
        VertexArray vertexArray;
    };

    Resource::Status build() override;

    void destroy() override {
        //TODO Unload the mesh resource
    }

    void render();

private:
    FbxScene *loadFromFile() const;

    void processMeshNode(FbxNode *node, FbxGeometryConverter &converter);

    void getMeshAttributes(FbxMesh *mesh,
                           std::vector<unsigned int> &indices,
                           std::vector<float> &vertices,
                           std::vector<float> &normals,
                           std::vector<float> &texCoords,
                           std::vector<float> &tangents
    ) const;

    std::vector<MeshInstance> m_meshes;
};
