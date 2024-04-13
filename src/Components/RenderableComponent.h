#pragma once
#include "../Renderer/Camera.h"
#include "../Renderer/Shader.h"
#include <glm/mat4x4.hpp>

class RenderableComponent
{
public:
    virtual void render() = 0;
    virtual glm::mat4 &getTransform() = 0;
    virtual void setShaderMaterialParams(Shader &shader) = 0;
};
