#include "Scene.h"

ModelComponent &Scene::createModelComponent(std::string modelFilename, std::string textureFilename)
{
    ModelComponent *model = new ModelComponent();
    model->model.createFromFile(modelFilename);
    model->textureId = Texture2D::createTexture(textureFilename);

    renderables.push_back(model);

    return *model;
}

void Scene::render(Camera &camera, Shader &shader)
{
    shader.setUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());
    for (const auto &renderable : renderables)
    {
        shader.setUniform("modelMatrix", renderable->getTransform());
        // renderable->setShaderMaterialParams(shader);
        renderable->render();
    }
}

void Scene::renderDepth(Camera &camera, Shader &shader)
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

void Scene::renderWithTransform(Shader &shader)
{
    for (const auto &renderable : renderables)
    {
        shader.setUniform("modelMatrix", renderable->getTransform());
        renderable->render();
    }
}

Light *Scene::createPointLight(glm::vec3 pos, glm::vec3 color, float falloff, float intensity)
{
    Light *light = new Light();

    light->type = Light::POINT;
    light->position = pos;
    light->color = color;
    light->distAttenMin = 0;
    light->distAttenMax = falloff;
    light->intensity = intensity;
    light->doesCastShadows = false;

    lights.push_back(light);

    return light;
}

Light *Scene::createSpotLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float beamAngle, float falloff, float intensity)
{
    Light *light = new Light();

    light->type = Light::SPOT;
    light->position = pos;
    light->direction = glm::normalize(direction);
    light->color = color;
    light->distAttenMin = 0;
    light->distAttenMax = falloff;
    light->intensity = intensity;
    light->beamAngle = beamAngle;
    light->doesCastShadows = false;

    lights.push_back(light);

    return light;
}