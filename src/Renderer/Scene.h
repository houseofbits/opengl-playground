#pragma once

#include "Cube.h"
#include "Plane.h"
#include "Camera.h"
#include "../Components/ModelComponent.h"
#include "Texture2D.h"
#include <list>

class Scene
{
public:
    std::list<RenderableComponent *> renderables;

    ModelComponent &createModelComponent(std::string modelFilename, std::string textureFilename)
    {
        ModelComponent *model = new ModelComponent();
        model->model.createFromFile(modelFilename);
        model->textureId = Texture2D::createTexture(textureFilename);

        renderables.push_back(model);

        return *model;
    }

    void render(Camera &camera, Shader &shader)
    {
        shader.setUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());
        for (const auto &renderable : renderables)
        {
            shader.setUniform("modelMatrix", renderable->getTransform());
            renderable->setShaderMaterialParams(shader);
            renderable->render();
        }
    }

    void renderDepth(Camera &camera, Shader &shader)
    {
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);

        shader.setUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());
        for (const auto &renderable : renderables)
        {
            shader.setUniform("modelMatrix", renderable->getTransform());
            renderable->render();
        }

        glCullFace(GL_BACK);
    }
};