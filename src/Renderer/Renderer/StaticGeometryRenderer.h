#pragma once

#include "../Texture/TextureCube.h"
#include "../Texture/TextureCubeRenderTarget.h"
#include "BaseRenderer.h"

class StaticGeometryRenderer : public BaseRenderer
{
public:
    explicit StaticGeometryRenderer(RenderManager*);

    void init();
    void render(Scene &scene, Camera& camera);

    Shader materialShader;
};

