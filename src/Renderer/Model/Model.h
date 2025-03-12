#pragma once

#include "VertexArray.h"
#include <string>
#include <glm/mat4x4.hpp>

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
};
