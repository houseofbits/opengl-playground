#pragma once

#include "BaseRenderer.h"

class StaticGeometryRenderer : public BaseRenderer
{
public:
    explicit StaticGeometryRenderer(RenderManager*);

    void init();
    void render(Scene &scene);

    Shader materialShader;
};

