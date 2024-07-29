#include "ShadowMapRenderer.h"
#include "../../Include.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ShadowMapRenderer::ShadowMapRenderer(RenderManager *manager) :
                                                               BaseRenderer(manager),
                                                               depthShader(),
//                                                               debugImageRenderer(),
                                                               lightIndexesUniform(),
                                                               numLightsPerPass(0)
{
    ShaderSourceLoader::registerGlobal("MAX_LIGHTVIEWS_PER_PASS", MAX_LIGHTVIEWS_PER_PASS);
}

void ShadowMapRenderer::init(unsigned int uniformBindingIndex)
{
    depthShader.loadProgram(
        "resources/shaders/shadowAtlas.vert",
        "resources/shaders/shadowAtlas.frag",
        "resources/shaders/shadowAtlas.geom");

//    debugImageRenderer.init(glm::vec4(-1, -1, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageDepth.frag");
//    debugImageRenderer.textureId = renderManager->atlasManager.getTextureId(TextureAtlasManager::ATLAS_SHADOW_DEPTH);

    lightIndexesUniform.create(MAX_LIGHTVIEWS_PER_PASS, uniformBindingIndex);
}

void ShadowMapRenderer::renderShadowAtlas()
{
//    debugImageRenderer.draw();
}

void ShadowMapRenderer::renderScene(Scene &scene)
{
    depthShader.setUniform("lightIndexes", lightIndexesUniform.getBufferId());
    depthShader.setUniform("numLightsPerPass", numLightsPerPass);
    prepareViewports();
    scene.renderWithTransform(depthShader);
}

void ShadowMapRenderer::render(Scene &scene)
{
    depthShader.use();
    renderManager->atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).bindRenderTarget();
    renderManager->atlasManager.bindAll(depthShader);
    renderManager->lightsUniformBuffer.bind(depthShader);

    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);

    lightIndexesUniform.bind();
    unsigned int numLights = renderManager->lightsUniformBuffer.getNumActiveLights();
    numLightsPerPass = 0;
    for(unsigned int i=0; i<numLights; i++) {
        if (renderManager->lightsUniformBuffer.get(i).doesCastShadows > 0) {
            lightIndexesUniform.set(i, numLightsPerPass);
            numLightsPerPass++;
        }
        if (numLightsPerPass >= MAX_LIGHTVIEWS_PER_PASS) {
            lightIndexesUniform.update(0, numLightsPerPass);
            renderScene(scene);
            numLightsPerPass = 0;
        }
    }

    if (numLightsPerPass > 0) {
        lightIndexesUniform.update(0, numLightsPerPass);
        renderScene(scene);
    }

    glCullFace(GL_BACK);

    renderManager->atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).unbindRenderTarget();
}

void ShadowMapRenderer::prepareViewports()
{
    int viewportIndex = 0;
    for (unsigned int i = 0; i < numLightsPerPass; i++)
    {
        unsigned int lightIndex = lightIndexesUniform.get(i);
        LightUniform &light = renderManager->lightsUniformBuffer.get(lightIndex);
        if (light.doesCastShadows == 1)
        {
            glm::uvec4 rect = renderManager->atlasManager.getRegionRect(TextureAtlasManager::ATLAS_SHADOW_DEPTH, light.shadowAtlasIndex);

            glViewportIndexedf(viewportIndex, (float)rect.x, (float)rect.y, (float)rect.z, (float)rect.w);

            viewportIndex++;
        }
    }
}
