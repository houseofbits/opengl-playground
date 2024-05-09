#include "Scene.h"

Scene::Scene(): camera(), renderables(), lights()
{

}

void Scene::render(Shader &shader)
{
    for (const auto &renderable : renderables)
    {
        shader.setUniform("modelMatrix", renderable->getTransform());
        renderable->setShaderMaterialParams(shader);
        renderable->render();
    }
}

void Scene::renderWithTransform(Shader &shader)
{
    for (const auto &renderable : renderables)
    {
        shader.setUniform("modelMatrix", renderable->getTransform());
        renderable->render();
    }
}

ModelComponent &Scene::createModel()
{
    auto *model = new ModelComponent();
    renderables.push_back(model);

    return *model;
}

Light &Scene::createLight()
{
    auto *light = new Light();
    lights.push_back(light);

    return *light;
}

void Scene::clear()
{
    //TODO Proper Scene cleanup
    renderables.clear();
    lights.clear();
}