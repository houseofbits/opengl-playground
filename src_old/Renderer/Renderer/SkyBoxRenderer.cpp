#include "SkyBoxRenderer.h"
#include "../Texture/TextureLoader.h"
#include "../RenderManager.h"

SkyBoxRenderer::SkyBoxRenderer(RenderManager *manager) : BaseRenderer(manager), texture(), shader(), box() {
}

void SkyBoxRenderer::init() {
    texture = TextureLoader::loadCube("resources/textures/sky2/*.png", true);
    shader.loadProgram("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
    box.create(10.0);
}

void SkyBoxRenderer::render(Scene &scene, Camera& camera) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    shader.use();
    glActiveTexture(GL_TEXTURE4);
    texture.bind();
//    glBindTexture(GL_TEXTURE_CUBE_MAP, renderManager->cubeRenderTarget.textureId);
    shader.setUniform("skyboxTexture", 4);
    camera.bind(shader);
    box.draw();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
