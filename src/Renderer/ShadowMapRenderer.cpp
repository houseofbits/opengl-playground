#include "../Include.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ShadowMapRenderer::ShadowMapRenderer()
{
    ShaderSourceLoader::registerGlobal("MAX_LIGHTVIEWS_PER_PASS", MAX_LIGHTVIEWS_PER_PASS);
}

void ShadowMapRenderer::init()
{
    depthShader.loadProgram(
        "resources/shaders/shadowAtlas.vert",
        "resources/shaders/shadowAtlas.frag",
        "resources/shaders/shadowAtlas.geom");

    shadowAtlas.create(RenderTarget::TARGET_DEPTH, ATLAS_WIDTH, ATLAS_HEIGHT);
    // shadowAtlas.create(RenderTarget::TARGET_COLOR, ATLAS_WIDTH, ATLAS_HEIGHT);
    atlasGraph.generate(ATLAS_WIDTH, ATLAS_HEIGHT);
    // atlasGraph.debug();
    // shadowAtlas.isClearBuffersEnabled = false;

    // std::cout << "LightViewUniform size: " << sizeof(LightViewUniform) << std::endl;

    glGenBuffers(1, &lightViewsUniformBufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, lightViewsUniformBufferId);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightViewUniform) * MAX_LIGHTVIEWS_PER_PASS, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightViewsUniformBufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShadowMapRenderer::generateShadowAtlasViews(std::list<Light *> &lights)
{
    atlasGraph.clearAll();
    unsigned int regionSize = 512;
    unsigned int posX = 0;
    unsigned int index = 0;
    for (const auto &light : lights)
    {
        if (light->doesCastShadows && light->type == Light::SPOT)
        {
            int atlasIndex = atlasGraph.occupyFirstAvailable(regionSize, regionSize);
            AtlasNode &node = atlasGraph.nodes[atlasIndex];

            lightViewUniformData[index].lightProjectionViewMatrix = light->getProjectionViewMatrix();
            lightViewUniformData[index].atlasPos = glm::vec2((float)node.left / (float)ATLAS_WIDTH, (float)node.top / (float)ATLAS_HEIGHT);
            lightViewUniformData[index].atlasSize = glm::vec2((float)node.width / (float)ATLAS_WIDTH, (float)node.height / (float)ATLAS_HEIGHT);
            lightViewUniformData[index].lightIndex = light->uniformBufferIndex;

            atlasViewports[index] = glm::uvec4(node.left, node.top, node.width, node.height);

            index++;
            posX += regionSize;
            if (posX >= ATLAS_WIDTH)
            {
                break;
            }
        }
    }

    numberOfLightViews = index;

    glBindBuffer(GL_UNIFORM_BUFFER, lightViewsUniformBufferId);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightViewUniform) * numberOfLightViews, lightViewUniformData);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShadowMapRenderer::renderShadowAtlas(Scene &scene)
{
    shadowAtlas.beginRender();

    for (unsigned int index = 0; index < numberOfLightViews; index++)
    {
        glViewportIndexedf(
            index,
            atlasViewports[index].x,
            atlasViewports[index].y,
            atlasViewports[index].z,
            atlasViewports[index].w);
    }

    depthShader.use();
    setShaderAttributes(depthShader);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    scene.renderWithTransform(depthShader);

    shadowAtlas.end();
    glCullFace(GL_BACK);
}

void ShadowMapRenderer::setShaderAttributes(Shader &shader)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, Renderer::LIGHT_VIEWS_UNIFORM_BINDING_INDEX, lightViewsUniformBufferId);
    shader.setUniform("lightViews", lightViewsUniformBufferId);
    shader.setUniform("numberOfLightViews", numberOfLightViews);
}