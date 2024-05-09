#pragma once

#include "Cube.h"
#include "Plane.h"
#include "Camera.h"
#include "Light.h"
#include "../Components/ModelComponent.h"
#include "Texture2D.h"
#include "TextureAtlasManager.h"
#include <list>

class Scene
{
public:
    Scene();

    Camera camera;
    std::list<RenderableComponent *> renderables;
    std::list<Light *> lights;

    ModelComponent &createModelComponent(TextureAtlasManager* atlasManager, std::string modelFilename, std::string textureFilename);
    Light &createPointLight(glm::vec3 pos, glm::vec3 color, float falloff, float intensity = 1.);
    Light &createSpotLight(TextureAtlasManager* atlasManager, glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float beamAngle, float falloff, float intensity = 1.);

    void render( Shader &shader);
    void renderDepth(Shader &shader);
    void renderWithTransform(Shader &shader);
};