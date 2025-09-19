#pragma once

#include "WireModel.h"

class WireSphere : public WireModel
{
public:
    WireSphere();

    void create(glm::vec4 color, float size = 1.0f);
};
