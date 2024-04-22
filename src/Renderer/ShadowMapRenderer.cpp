#include "../Include.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ShadowMapRenderer::ShadowMapRenderer() : depthShader(), shadowAtlas(), atlasGraph(), debugImageRenderer()
{
    ShaderSourceLoader::registerGlobal("MAX_LIGHTVIEWS_PER_PASS", MAX_LIGHTVIEWS_PER_PASS);
}

void ShadowMapRenderer::init()
{
    shadowAtlas.create(RenderTarget::TARGET_DEPTH, ATLAS_WIDTH, ATLAS_HEIGHT);
    // shadowAtlas.create(RenderTarget::TARGET_COLOR, ATLAS_WIDTH, ATLAS_HEIGHT);
    atlasGraph.generate(ATLAS_WIDTH, ATLAS_HEIGHT);

    generateAtlasRegionUniformBuffer();

    depthShader.loadProgram(
        "resources/shaders/shadowAtlas.vert",
        "resources/shaders/shadowAtlas.frag",
        "resources/shaders/shadowAtlas.geom");

    debugImageRenderer.init(glm::vec4(-1, -1, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageDepth.frag");
    debugImageRenderer.textureId = shadowAtlas.targetTextureId;

    // atlasGraph.debug();
}

void ShadowMapRenderer::beginRender()
{
    shadowAtlas.beginRender();
    depthShader.use();
}

void ShadowMapRenderer::renderShadowAtlas(Scene &scene)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    scene.renderWithTransform(depthShader);

    shadowAtlas.end();
    glCullFace(GL_BACK);
}

void ShadowMapRenderer::debugRender()
{
    debugImageRenderer.draw();
}

void ShadowMapRenderer::generateAtlasRegionUniformBuffer()
{
    glm::vec4 *data = atlasGraph.getRectangleArray(ATLAS_WIDTH, ATLAS_HEIGHT);
    glGenBuffers(1, &shadowAtlasRegionBufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, shadowAtlasRegionBufferId);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * atlasGraph.size, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, shadowAtlasRegionBufferId);

    data[0] = glm::vec4(1, 0, 0, 1);
    data[1] = glm::vec4(0, 1, 0, 1);
    data[2] = glm::vec4(0, 0, 1, 1);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4) * atlasGraph.size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    delete data;

    ShaderSourceLoader::registerGlobal("NUM_SHADOW_ATLAS_REGIONS", atlasGraph.size);
}