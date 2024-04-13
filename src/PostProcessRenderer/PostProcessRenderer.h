#pragma once

#include "../Renderer/Shader.h"
#include "../Renderer/VertexArray.h"
#include <string>
#include "glm/vec4.hpp"

class PostProcessRenderer : public VertexArray
{
public:
    Shader shader;
    unsigned int textureId;

    void init(glm::vec4 rect, std::string vertexProgramFileName, std::string fragmentProgramFileName);
    void draw();
};