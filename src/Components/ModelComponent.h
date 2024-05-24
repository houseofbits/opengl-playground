#pragma once

#include "../Renderer/Camera/Camera.h"
#include "../Renderer/Model/Model.h"
#include "../Renderer/Shader/Shader.h"
#include "RenderableComponent.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
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

    glm::mat4 &getTransform() override { return transform; }
    void setShaderMaterialParams(Shader &shader) override
    {
        shader.setUniform("diffuseTextureId", diffuseTextureId);
        shader.setUniform("normalTextureId", normalTextureId);
        shader.setUniform("specularTextureId", specularTextureId);
        shader.setUniform("diffuseColor", diffuseColor);
        shader.setUniform("selfIllumination", selfIllumination);
        shader.setUniform("doesReceiveShadows", doesReceiveShadows);
        shader.setUniform("specularPower", specularPower);
        shader.setUniform("specularColor", specularColor);
    }

    glm::mat4 transform;
    Model model;

    unsigned int diffuseTextureId;
    unsigned int normalTextureId;
    unsigned int specularTextureId;
    glm::vec4 diffuseColor;
    float selfIllumination;
    bool doesReceiveShadows;
    unsigned int specularPower;
    glm::vec3 specularColor;

};
