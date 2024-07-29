#pragma once

class RenderManager;

class BaseRenderer
{
protected:
    RenderManager* renderManager;

public:
    explicit BaseRenderer(RenderManager* manager) : renderManager(manager)
    {

    }
};
