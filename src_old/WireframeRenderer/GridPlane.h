#pragma once

#include "LineMesh.h"
#include <glm/vec3.hpp>

class GridPlane : public LineMesh {
public:
    void create(float width, float height, int subdivision);
    void draw();
};