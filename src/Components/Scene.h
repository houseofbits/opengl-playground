#pragma once

#include "../Renderer/Camera/Camera.h"
#include "../Renderer/Light/Light.h"
#include "../Renderer/Model/Cube.h"
#include "../Renderer/Model/Plane.h"
#include "../Renderer/Texture/Texture2D.h"
#include "../Renderer/Texture/TextureAtlasManager.h"
#include "ModelComponent.h"
#include <list>

class Scene {
public:
    Scene();

    Camera camera;
    std::list<RenderableComponent *> renderables;
    std::list<Light *> lights;

    ModelComponent &createModel();
    Light &createLight();

    void render(Shader &shader);
    void renderWithTransform(Shader &shader);
    void clear();
};