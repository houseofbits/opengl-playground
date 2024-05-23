#pragma once

#include "BaseRenderer.h"
#include "TextureCube.h"

class StaticGeometryRenderer : public BaseRenderer
{
public:
    explicit StaticGeometryRenderer(RenderManager*);

    void init();
    void render(Scene &scene);

    TextureCube skyboxTexture;
    Cube skyBox;
    Shader skyShader;

    Shader materialShader;
};

