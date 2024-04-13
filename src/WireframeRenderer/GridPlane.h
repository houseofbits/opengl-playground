#pragma once

#include <glm/vec3.hpp>
#include "WireFrame.h"

class GridPlane : public WireFrame
{
public:
    void create(float width, float height, int subdivision);
    void draw();
};