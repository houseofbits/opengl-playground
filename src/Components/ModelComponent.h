#pragma once

#include "RenderableComponent.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Model.h"
#include "../Renderer/Shader.h"
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

class ModelComponent : public RenderableComponent
{
public:
    ModelComponent();

    void load(std::string filename); // remove, should be created with factory or via config
    void render();
    ModelComponent &resetTransform()
    {
        transform = glm::mat4(1.0);

        return *this;
    }
    ModelComponent &setPosition(glm::vec3 pos)
    {
        transform = glm::translate(transform, pos);

        return *this;
    }
    ModelComponent &setScale(glm::vec3 pos)
    {
        transform = glm::scale(transform, pos);

        return *this;
    }
    ModelComponent &setRotation(glm::vec3 pos)
    {
        transform = glm::rotate(transform, glm::radians(pos.x), glm::vec3(1, 0, 0));
        transform = glm::rotate(transform, glm::radians(pos.y), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(pos.z), glm::vec3(0, 0, 1));

        return *this;
    }

    glm::mat4 &getTransform() { return transform; }
    void setShaderMaterialParams(Shader &shader)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        shader.setUniform("texture1", 0);
    }

    glm::mat4 transform;
    unsigned int textureId;
    Model model;
};
