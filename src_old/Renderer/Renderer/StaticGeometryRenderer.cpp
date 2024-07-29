#include "../../Include.h"
#include "../Texture/TextureLoader.h"

StaticGeometryRenderer::StaticGeometryRenderer(RenderManager *manager) : BaseRenderer(manager), materialShader(),
                                                                         materialShaderSimplified(),
                                                                         materialShaderDepthPrePass() {
}

void StaticGeometryRenderer::init() {
    materialShader.loadProgram(
            "resources/shaders/lighting.vert",
            "resources/shaders/lighting.frag",
            "resources/shaders/lighting.geom");

    materialShaderSimplified.loadProgram(
            "resources/shaders/lighting.vert",
            "resources/shaders/lightingSimplified.frag",
            "resources/shaders/lighting.geom");

    materialShaderDepthPrePass.loadProgram(
            "resources/shaders/depthPrePass.vert",
            "resources/shaders/depthPrePass.frag");
}

void StaticGeometryRenderer::render(Scene &scene, Camera &camera) {
    materialShader.use();
    renderManager->atlasManager.bindAll(materialShader);
    camera.bind(materialShader);
    renderManager->lightsUniformBuffer.bind(materialShader);
//    renderManager->environmentProbeUniformBuffer.bind(materialShader);

    glActiveTexture(GL_TEXTURE4);
//    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, renderManager->environmentProbeRenderer.cubeRenderTarget.textureId);
    materialShader.setUniform("envProbeArray", 4);

    scene.render(materialShader);
}

void StaticGeometryRenderer::renderSimplified(Scene &scene, Camera &camera) {
    materialShaderSimplified.use();
    renderManager->atlasManager.bindAll(materialShaderSimplified);
    camera.bind(materialShaderSimplified);
    renderManager->lightsUniformBuffer.bind(materialShaderSimplified);

    glActiveTexture(GL_TEXTURE4);
    //    glBindTexture(GL_TEXTURE_CUBE_MAP, renderManager->cubeRenderTarget.textureId);
    renderManager->skyBoxRenderer.texture.bind();
    materialShaderSimplified.setUniform("skyboxTexture", 4);

    scene.render(materialShaderSimplified);
}

void StaticGeometryRenderer::renderDepthPrePass(Scene &scene, Camera& camera)
{
    materialShaderDepthPrePass.use();
    camera.bind(materialShaderDepthPrePass);
    scene.render(materialShaderDepthPrePass);
}