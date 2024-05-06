#include "../Include.h"

StaticGeometryRenderer::StaticGeometryRenderer(RenderManager *manager) : BaseRenderer(manager)
{

}

void StaticGeometryRenderer::init()
{
    materialShader.loadProgram("resources/shaders/lighting.vert", "resources/shaders/lighting2.frag");
}

void StaticGeometryRenderer::render(Scene &scene)
{
    glViewport(0, 0, renderManager->window->viewportWidth, renderManager->window->viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    materialShader.use();
    renderManager->camera->bind(materialShader);
    renderManager->lightsUniformBuffer.bind(materialShader);
    renderManager->atlasManager.bindAll(materialShader);

    scene.render(materialShader);
}