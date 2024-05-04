#include "Renderer.h"
#include "Camera.h"
#include "Texture2D.h"
#include <GL/glew.h>
#include <iostream>
#include <math.h>
#include "../Helper/ShaderSourceLoader.h"

Renderer::Renderer() : activeCamera(0), atlasManager(), lightsUniformBuffer()
{
    ShaderSourceLoader::registerGlobal("LIGHTS_UNIFORM_BINDING_INDEX", LIGHTS_UNIFORM_BINDING_INDEX);
    ShaderSourceLoader::registerGlobal("LIGHT_VIEWS_UNIFORM_BINDING_INDEX", LIGHT_VIEWS_UNIFORM_BINDING_INDEX);

    // std::cout << "LightUniform size: " << sizeof(LightUniform) << std::endl;
    // std::cout << "size uint: " << sizeof(unsigned int) << std::endl;
    // std::cout << "size float: " << sizeof(float) << std::endl;
    // std::cout << "size vec4: " << sizeof(glm::vec4) << std::endl;
    // std::cout << "size vec3: " << sizeof(glm::vec3) << std::endl;
    // std::cout << "size vec2: " << sizeof(glm::vec2) << std::endl;
    // std::cout << "size mat4: " << sizeof(glm::mat4) << std::endl;
}

void Renderer::init(Camera *camera)
{
    activeCamera = camera;
    lightsUniformBuffer.create(20);
    atlasManager.init();
    shadowMapRenderer.init();
    shadowMapRenderer.debugImageRenderer.textureId = atlasManager.getTextureId(TextureAtlasManager::ATLAS_SHADOW_DEPTH);
}

void Renderer::setShaderAttributes(Shader &shader)
{
    shader.setUniform("viewPosition", activeCamera->getPosition());
    lightsUniformBuffer.bind(shader);
    atlasManager.bindAll(shader);
}

void Renderer::updateLights(Scene &scene)
{
    lightsUniformBuffer.update(scene, atlasManager);
}

void Renderer::renderShadowAtlas(Scene &scene)
{
    atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).bindRenderTarget();
    shadowMapRenderer.depthShader.use();
    setShaderAttributes(shadowMapRenderer.depthShader);

    int viewportIndex = 0;
    for (unsigned int i = 0; i < lightsUniformBuffer.getNumActiveLights(); i++)
    {
        LightUniform &light = lightsUniformBuffer.get(i);
        if (light.doesCastShadows == 1)
        {
            glm::uvec4 rect = atlasManager.getRegionRect(TextureAtlasManager::ATLAS_SHADOW_DEPTH, light.shadowAtlasIndex);

            glViewportIndexedf(viewportIndex, rect.x, rect.y, rect.z, rect.w);

            viewportIndex++;
        }
    }

    shadowMapRenderer.renderShadowAtlas(scene);

    atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).unbindRenderTarget();
}

void Renderer::printDebugShadowMaps()
{
    for (unsigned int lightIndex = 0; lightIndex < lightsUniformBuffer.getNumActiveLights(); lightIndex++)
    {
        std::cout << "Light "
                  << lightIndex
                  << ", Atlas index "
                  << lightsUniformBuffer.get(lightIndex).shadowAtlasIndex
                  << std::endl;
        ;
    }
}