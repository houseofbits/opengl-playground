#pragma once
#include "RenderQueue.h"
#include "../Model/Model.h"

class RenderQueueBuilder {
public:
    RenderQueueBuilder();

    static void build(RenderQueue &queue, const Model &model, RenderShader &shader, const glm::mat4 &worldTransform);
};
