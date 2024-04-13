#include "WireframeRenderer.h"

void WireframeRenderer::init()
{
    shader.loadProgram("resources/shaders/grid.vert", "resources/shaders/grid.frag");
    gridPlane.create(1000, 1000, 100);
}

void WireframeRenderer::draw(Renderer *renderer)
{
    shader.use();
    shader.setUniform("viewProjectionMatrix", renderer->activeCamera->getProjectionViewMatrix());
    gridPlane.draw();
}