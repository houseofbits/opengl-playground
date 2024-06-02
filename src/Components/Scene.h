#pragma once

#include "../Renderer/Camera/Camera.h"
#include "../Renderer/EnvironmentProbe/EnvironmentProbe.h"
#include "../Renderer/Light/Light.h"
#include "../Renderer/Model/Cube.h"
#include "../Renderer/Model/Plane.h"
#include "../Renderer/Texture/Texture2D.h"
#include "../Renderer/TextureAtlas/TextureAtlasManager.h"
#include "ModelComponent.h"
#include <list>

class Scene {
public:
    Scene();

    std::string filename;
    Camera camera;
    std::list<RenderableComponent *> renderables;
    std::list<Light *> lights;
    std::list<EnvironmentProbe *> probes;

    ModelComponent &createModel();
    Light &createLight();
    EnvironmentProbe &createProbe();

    void render(Shader &shader);
    void renderWithTransform(Shader &shader);
    void clear();
};