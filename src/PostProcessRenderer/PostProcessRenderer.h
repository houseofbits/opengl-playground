#pragma once

#include "../Renderer/Model/VertexArray.h"
#include "../Renderer/Shader/Shader.h"
#include "glm/vec4.hpp"
#include <string>

class PostProcessRenderer : public VertexArray
{
public:
    Shader shader;
    unsigned int textureId;

    void init(glm::vec4 rect, std::string vertexProgramFileName, std::string fragmentProgramFileName);
    void draw();

    void render();
};