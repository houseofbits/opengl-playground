#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Queue/RenderQueue.h"

class RenderQueueResource : public Resource, public RenderQueue {
public:
    RenderQueueResource();
};
