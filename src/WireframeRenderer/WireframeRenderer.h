#pragma once

#include "../Renderer/Shader.h"
#include "GridPlane.h"

class WireframeRenderer
{
public:
    Shader shader;
    GridPlane gridPlane;

    void init();
    void draw();
};