#pragma once

#include "../Renderer/Camera.h"
#include "../Renderer/Cube.h"
#include "../Renderer/Light.h"
#include "../Renderer/Plane.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/TextureAtlasManager.h"
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