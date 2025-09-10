#pragma once

#include "VertexArray.h"
#include <string>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

class Model final : public VertexArray {
public:
    class MeshInstance {
    public:
        std::string name{};
        int materialIndex{-1};
        glm::mat4 modelMatrix{1.0};
        VertexArray vertexArray{};
    };

    std::vector<MeshInstance *> m_meshes{};

    //new stuff
    class MeshNode {
    public:
        std::string name{};
        int materialIndex{-1};
        glm::mat4 modelMatrix{1.0};
        glm::vec3 scaling{1.0};
        int offset{0};
        int size{0};
    };

    GLuint m_vertexArrayObject{0};
    GLuint m_vertexBufferObject{0};
    GLuint m_elementBufferObject{0};
    std::vector<MeshNode> m_meshNodes{};
};
