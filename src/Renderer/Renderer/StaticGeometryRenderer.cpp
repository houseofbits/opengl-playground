#include "../../Include.h"
#include "../Texture/TextureLoader.h"

StaticGeometryRenderer::StaticGeometryRenderer(RenderManager *manager) : BaseRenderer(manager) {
}

void StaticGeometryRenderer::init() {
    materialShader.loadProgram(
            "resources/shaders/lighting.vert",
                        "resources/shaders/lightingSimplified.frag",  //Used to render scene into reflection probes
//            "resources/shaders/lighting.frag",
            "resources/shaders/lighting.geom");
}

void StaticGeometryRenderer::render(Scene &scene, Camera& camera) {
    materialShader.use();
    renderManager->atlasManager.bindAll(materialShader);
    camera.bind(materialShader);
    renderManager->lightsUniformBuffer.bind(materialShader);

    glActiveTexture(GL_TEXTURE4);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, renderManager->cubeRenderTarget.textureId);
    renderManager->skyBoxRenderer.texture.bind();
    materialShader.setUniform("skyboxTexture", 4);

    scene.render(materialShader);
}
