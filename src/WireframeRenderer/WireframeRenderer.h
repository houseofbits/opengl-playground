#pragma once

#include "../Renderer/Shader.h"
#include "GridPlane.h"
#include "../Renderer/Renderer.h"

class WireframeRenderer
{
public:
    Shader shader;
    GridPlane gridPlane;

    void init();
    void draw(Renderer *renderer);
};