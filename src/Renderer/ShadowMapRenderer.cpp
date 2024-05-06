#include "../Include.h"
#include "ShadowMapRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ShadowMapRenderer::ShadowMapRenderer(RenderManager *manager) : BaseRenderer(manager), depthShader(), debugImageRenderer()
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
    debugImageRenderer.textureId = renderManager->atlasManager.getTextureId(TextureAtlasManager::ATLAS_SHADOW_DEPTH);
}

void ShadowMapRenderer::renderShadowAtlas()
{
    debugImageRenderer.draw();
}

void ShadowMapRenderer::render(Scene &scene)
{
    depthShader.use();
    renderManager->atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).bindRenderTarget();
    renderManager->atlasManager.bindAll(depthShader);
    renderManager->lightsUniformBuffer.bind(depthShader);

    prepareViewports();

//    glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    scene.renderWithTransform(depthShader);

//    glCullFace(GL_BACK);

    renderManager->atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).unbindRenderTarget();
}

void ShadowMapRenderer::prepareViewports()
{
    int viewportIndex = 0;
    for (unsigned int i = 0; i < renderManager->lightsUniformBuffer.getNumActiveLights(); i++)
    {
        LightUniform &light = renderManager->lightsUniformBuffer.get(i);
        if (light.doesCastShadows == 1)
        {
            glm::uvec4 rect = renderManager->atlasManager.getRegionRect(TextureAtlasManager::ATLAS_SHADOW_DEPTH, light.shadowAtlasIndex);

            glViewportIndexedf(viewportIndex, (float)rect.x, (float)rect.y, (float)rect.z, (float)rect.w);

            viewportIndex++;
        }
    }
}
