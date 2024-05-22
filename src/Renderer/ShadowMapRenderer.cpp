#include "../Include.h"
#include "ShadowMapRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ShadowMapRenderer::ShadowMapRenderer(RenderManager *manager) :
                                                               BaseRenderer(manager),
                                                               depthShader(),
                                                               debugImageRenderer(),
                                                               lightIndexesUniform(),
                                                               numLightsPerPass(0)
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

    lightIndexesUniform.create(MAX_LIGHTVIEWS_PER_PASS, UniformIndexBuffer::DYNAMIC, LIGHT_INDEXES_UNIFORM_BINDING_INDEX);
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

    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);

    unsigned int numPages = calculateNumPages();
    for(unsigned int page=0; page<numPages; page++)
    {
        prepareIndexes(page);

        lightIndexesUniform.bind();
        depthShader.setUniform("lightIndexes", lightIndexesUniform.getBufferId());
        depthShader.setUniform("numLightsPerPass", numLightsPerPass);

        prepareViewports();

        scene.renderWithTransform(depthShader);
    }

    glCullFace(GL_BACK);

    renderManager->atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).unbindRenderTarget();
}

void ShadowMapRenderer::prepareIndexes(unsigned int page)
{
//    std::cout<<"page: "<<page<<std::endl;
    unsigned int numLights = renderManager->lightsUniformBuffer.getNumActiveLights();
    unsigned int offset = MAX_LIGHTVIEWS_PER_PASS * page;
    unsigned int size = offset + MAX_LIGHTVIEWS_PER_PASS;
    if (size > numLights) {
        size = numLights;
    }
    if (offset < size) {
        unsigned int a = 0;
        for (unsigned int i = offset; i < size; i++) {
//            std::cout<<" "<<a<<": "<<i<<std::endl;
            lightIndexesUniform.set(i, a);
            a++;
        }
        numLightsPerPass = a;
//        std::cout<<" num:"<<numLightsPerPass<<std::endl;
    }
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
unsigned int ShadowMapRenderer::calculateNumPages() {

    return (unsigned int) ceil((float)renderManager->lightsUniformBuffer.getNumActiveLights() / (float)MAX_LIGHTVIEWS_PER_PASS);
}
