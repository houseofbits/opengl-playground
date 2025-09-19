#pragma once
#include "WireModel.h"

class WireCube : public WireModel
{
public:
    WireCube();

    void create(glm::vec4 color, float size = 1.0f);
};
