#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "RenderTarget.h"
#include <list>
#include <iostream>
#include <glm/vec3.hpp>

class ShadowMapRenderer
{
public:
    static const unsigned int MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS = 6;
    static const unsigned int ATLAS_WIDTH = 256 * 6;
    static const unsigned int ATLAS_HEIGHT = 256;

    typedef struct alignas(16)
    {
        glm::mat4 lightProjectionViewMatrix;
        glm::vec4 viewport;
        glm::vec4 atlasRect;
        // unsigned int lightIndex;  //Index into lights uniform buffer. therefor no need for lightPosition attribute
    } ShadowAtlasFragmentUniform;

    Shader depthShader;
    RenderTarget shadowAtlas;
    ShadowAtlasFragmentUniform shadowAtlasFragmentUniformData[MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS];
    unsigned int shadowAtlasFragmentsUniformBufferId;
    unsigned int numberOfFragments;

    void init()
    {
        depthShader.loadProgram(
            "resources/shaders/shadowAtlas.vert",
            "resources/shaders/shadowAtlas.frag",
            "resources/shaders/shadowAtlas.geom");

        shadowAtlas.create(RenderTarget::TARGET_DEPTH, ATLAS_WIDTH, ATLAS_HEIGHT);

        std::cout << sizeof(ShadowAtlasFragmentUniform) << std::endl;

        glGenBuffers(1, &shadowAtlasFragmentsUniformBufferId);
        glBindBuffer(GL_UNIFORM_BUFFER, shadowAtlasFragmentsUniformBufferId);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(ShadowAtlasFragmentUniform) * MAX_SHADOW_ATLAS_FRAGMENTS_PER_PASS, NULL, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, shadowAtlasFragmentsUniformBufferId);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void updateShadowAtlasLights(std::list<Light *> &lights)
    {
        unsigned int regionSize = 256;
        unsigned int posX = 0;
        unsigned int index = 0;
        for (const auto &light : lights)
        {
            if (light->doesCastShadows && light->type == Light::SPOT)
            {
                shadowAtlasFragmentUniformData[index].viewport = glm::vec4(posX, 0, regionSize, regionSize);
                shadowAtlasFragmentUniformData[index].lightProjectionViewMatrix = light->getProjectionViewMatrix();
                shadowAtlasFragmentUniformData[index].atlasRect = glm::vec4((float)posX / (float)ATLAS_WIDTH, 0, (float)regionSize / (float)ATLAS_WIDTH, (float)regionSize / (float)ATLAS_HEIGHT);

                light->setShadowAtlasPosition(posX / ATLAS_WIDTH, 0, regionSize / ATLAS_WIDTH, regionSize / ATLAS_HEIGHT);

                index++;
                posX += regionSize;
                if (posX >= ATLAS_WIDTH)
                {
                    break;
                }
            }
        }

        numberOfFragments = index;

        glBindBuffer(GL_UNIFORM_BUFFER, shadowAtlasFragmentsUniformBufferId);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShadowAtlasFragmentUniform) * numberOfFragments, shadowAtlasFragmentUniformData);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void beginRender(std::list<Light *> &lights)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        updateShadowAtlasLights(lights);

        shadowAtlas.beginRender();

        for (unsigned int index = 0; index < numberOfFragments; index++)
        {
            glViewportIndexedf(
                index,
                (unsigned int)shadowAtlasFragmentUniformData[index].viewport.x,
                (unsigned int)shadowAtlasFragmentUniformData[index].viewport.y,
                (unsigned int)shadowAtlasFragmentUniformData[index].viewport.z,
                (unsigned int)shadowAtlasFragmentUniformData[index].viewport.w);
        }

        depthShader.use();
        setShaderAttributes(depthShader);
    }

    void endRender()
    {
        shadowAtlas.end();
        glCullFace(GL_BACK);
    }

    void setShaderAttributes(Shader &shader)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, Renderer::LIGHT_VIEWS_UNIFORM_BINDING_INDEX, shadowAtlasFragmentsUniformBufferId);
        shader.setUniform("shadowFragments", shadowAtlasFragmentsUniformBufferId);
        shader.setUniform("numberOfFragments", numberOfFragments);
    }
};