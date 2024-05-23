#include "../Include.h"
#include "TextureLoader.h"

StaticGeometryRenderer::StaticGeometryRenderer(RenderManager *manager) : BaseRenderer(manager), skyboxTexture(), skyBox(), skyShader() {
}

void StaticGeometryRenderer::init() {
    materialShader.loadProgram(
            "resources/shaders/lighting.vert",
            "resources/shaders/lighting.frag",
            "resources/shaders/lighting.geom");
    //    materialShader.loadProgram(
    //            "resources/shaders/normals.vert",
    //            "resources/shaders/normals.frag",
    //            "resources/shaders/normals.geom");
    skyboxTexture = TextureLoader::loadCube("resources/textures/sky2/*.png", true);

    skyShader.loadProgram("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
    skyBox.create(10.0);
}

void StaticGeometryRenderer::render(Scene &scene) {
    glViewport(0, 0, renderManager->window->viewportWidth, renderManager->window->viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    skyShader.use();
    glActiveTexture(GL_TEXTURE4);
    skyboxTexture.bind();
    materialShader.setUniform("skyboxTexture", 4);
    scene.camera.bind(skyShader);
    skyBox.draw();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    materialShader.use();
    renderManager->atlasManager.bindAll(materialShader);
    scene.camera.bind(materialShader);
    renderManager->lightsUniformBuffer.bind(materialShader);

    glActiveTexture(GL_TEXTURE4);
    skyboxTexture.bind();
    materialShader.setUniform("skyboxTexture", 4);

    scene.render(materialShader);
}
