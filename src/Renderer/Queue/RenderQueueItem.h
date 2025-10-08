#pragma once

#include <glm/mat4x4.hpp>
#include <GL/glew.h>

class RenderShader;

class RenderQueueItem {
public:
    RenderQueueItem();

    RenderShader *shader;
    GLuint vertexArray;
    unsigned int offset{0};
    unsigned int size{0};
    unsigned int materialIndex;
    glm::mat4 modelMatrix{1.0};
};
