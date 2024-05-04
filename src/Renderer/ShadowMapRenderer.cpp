#include "../Include.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ShadowMapRenderer::ShadowMapRenderer() : depthShader(), debugImageRenderer()
{
    ShaderSourceLoader::registerGlobal("MAX_LIGHTVIEWS_PER_PASS", MAX_LIGHTVIEWS_PER_PASS);
    ShaderSourceLoader::registerGlobal("NUM_SHADOW_ATLAS_REGIONS", 1); // Remove
}

void ShadowMapRenderer::init()
{
    depthShader.loadProgram(
        "resources/shaders/shadowAtlas.vert",
        "resources/shaders/shadowAtlas.frag",
        "resources/shaders/shadowAtlas.geom");

    debugImageRenderer.init(glm::vec4(-1, -1, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageDepth.frag");
}

void ShadowMapRenderer::renderShadowAtlas(Scene &scene)
{
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    scene.renderWithTransform(depthShader);

    glCullFace(GL_BACK);
}

void ShadowMapRenderer::debugRender()
{
    debugImageRenderer.draw();
}

void ShadowMapRenderer::render(Scene &scene, TextureAtlasManager &atlasManager)
{

    // renderManager->atlasManager...etc

    //  atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).bindRenderTarget();
    //  shadowMapRenderer.depthShader.use();
    //  setShaderAttributes(shadowMapRenderer.depthShader);
    //  lightsUniformBuffer.bind(shader);

    // int viewportIndex = 0;
    // for (unsigned int i = 0; i < lightsUniformBuffer.getNumActiveLights(); i++)
    // {
    //     LightUniform &light = lightsUniformBuffer.get(i);
    //     if (light.doesCastShadows == 1)
    //     {
    //         glm::uvec4 rect = atlasManager.getRegionRect(TextureAtlasManager::ATLAS_SHADOW_DEPTH, light.shadowAtlasIndex);

    //         glViewportIndexedf(viewportIndex, rect.x, rect.y, rect.z, rect.w);

    //         viewportIndex++;
    //     }
    // }

    // shadowMapRenderer.renderShadowAtlas(scene);

    // atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).unbindRenderTarget();
}