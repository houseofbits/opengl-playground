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
    void renderSimplified(Scene &scene, Camera& camera);
    void renderDepthPrePass(Scene &scene, Camera& camera);

    Shader materialShader;
    Shader materialShaderSimplified;
    Shader materialShaderDepthPrePass;
};

