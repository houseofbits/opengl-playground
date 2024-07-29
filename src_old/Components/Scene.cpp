#include "Scene.h"

Scene::Scene(): camera(), renderables(), lights()
{

}

//TODO: Main material pass. Refactor or rename
void Scene::render(Shader &shader)
{
    for (const auto &renderable : renderables)
    {
        if (renderable->isSolidMaterial) {
            shader.setUniform("modelMatrix", renderable->getTransform());
            renderable->setShaderMaterialParams(shader);
            renderable->render();
        }
    }
}

//TODO This is for shadow map pass. Rename or refactor
void Scene::renderWithTransform(Shader &shader)
{
    for (const auto &renderable : renderables)
    {
        if (renderable->doesCastShadows) {
            shader.setUniform("modelMatrix", renderable->getTransform());
            renderable->render();
        }
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
//    probes.clear();
}

//EnvironmentProbe &Scene::createProbe() {
//    auto *probe = new EnvironmentProbe();
//    probes.push_back(probe);
//
//    return *probe;
//}
