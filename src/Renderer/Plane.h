#pragma once

#include "VertexArray.h"

class Plane : public VertexArray
{
public:
    void create(float xsize, float zsize, int xdivs = 1, int zdivs = 1, float smax = 1.0f, float tmax = 1.0f);
};