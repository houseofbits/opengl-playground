#include "../Include.h"
#include "RenderManager.h"


RenderManager::RenderManager(Window *w) : window(w),atlasManager(), shadowMapRenderer(this), staticGeometryRenderer(this), camera(nullptr)
{
}

void RenderManager::init()
{
    lightsUniformBuffer.create(20);
    atlasManager.init();
    shadowMapRenderer.init();
    staticGeometryRenderer.init();
}

void RenderManager::render(Scene& scene)
{
    lightsUniformBuffer.update(scene, atlasManager);
    shadowMapRenderer.render(scene);
    staticGeometryRenderer.render(scene);
}
void RenderManager::setCamera(Camera *cam) {
    camera = cam;
}
