#include "Scene.h"

Scene::Scene(): camera(), renderables(), lights()
{

}

ModelComponent &Scene::createModelComponent(TextureAtlasManager* atlasManager, std::string modelFilename, std::string textureFilename)
{
    auto *model = new ModelComponent();
    model->model.createFromFile(modelFilename);
//    model->textureId = Texture2D::createTexture(textureFilename);
    model->diffuseTextureId = atlasManager->loadTextureIntoAtlas(textureFilename, TextureAtlasManager::ATLAS_DIFFUSE);

//    std::cout<<textureFilename<<" "<<model->diffuseTextureId<<std::endl;

    renderables.push_back(model);

    return *model;
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

void Scene::renderDepth(Shader &shader)
{
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

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

Light &Scene::createPointLight(glm::vec3 pos, glm::vec3 color, float falloff, float intensity)
{
    auto *light = new Light();

    light->type = Light::POINT;
    light->position = pos;
    light->color = color;
    light->distAttenMin = 0;
    light->distAttenMax = falloff;
    light->intensity = intensity;
    light->doesCastShadows = false;
    light->projectionTextureId = 0;

    lights.push_back(light);

    return *light;
}

Light &Scene::createSpotLight(TextureAtlasManager* atlasManager, glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float beamAngle, float falloff, float intensity)
{
    auto *light = new Light();

    light->type = Light::SPOT;
    light->position = pos;
    light->direction = glm::normalize(direction);
    light->color = color;
    light->distAttenMin = 0;
    light->distAttenMax = falloff;
    light->intensity = intensity;
    light->beamAngle = beamAngle;
    light->doesCastShadows = false;
    light->projectionTextureId = atlasManager->loadTextureIntoAtlas("resources/textures/spot-proj-4.png", TextureAtlasManager::ATLAS_EFFECTS);

    lights.push_back(light);

    return *light;
}