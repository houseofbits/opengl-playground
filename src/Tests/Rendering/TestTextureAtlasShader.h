#pragma once

#include <vector>
#include <GL/glew.h>
#include "BaseRenderTest.h"
#include "../../Renderer/Texture.h"
#include "../../PostProcessRenderer/PostProcessRenderer.h"
#include "../../Renderer/TextureLoader.h"
#include "../../Renderer/UniformBuffer.h"
#include "../../Renderer/TextureAtlasManager.h"
#include "../../Helper/QuadTree.h"

class TestTextureAtlasShader : public BaseRenderTest
{
private:
    static const int numCubes = 30;

    TextureAtlasManager atlasManager;
    Camera camera;
    Shader shader;
    Cube cubes[numCubes];
    glm::mat4 cubeTransforms[numCubes];
    unsigned int cubeDiffuseTextureIds[numCubes];
    std::vector<int> textureIds;

    float randomFloat()
    {
        return (float)(rand()) / (float)(RAND_MAX);
    }

public:
    void setup(Window &window)
    {
        message("Test TextureAtlas - usage in shader");

        atlasManager.init();
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-512.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-512.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-256.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-256.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-128.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-128.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-128.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-128.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-128.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-512.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-256.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-256.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t-256.png", TextureAtlasManager::ATLAS_DIFFUSE));
        textureIds.push_back(atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-256.png", TextureAtlasManager::ATLAS_DIFFUSE));

        camera.setPosition(glm::vec3(50, 200, -500))
            .setAngles(glm::radians(-10.0f), glm::radians(-25.0f))
            .resize(window.viewportWidth, window.viewportHeight);

        shader.loadProgram("resources/test/shaders/basic.vert", "resources/test/shaders/textureAtlasMapping.frag");

        float spread = 500;
        for (unsigned int i = 0; i < numCubes; i++)
        {
            cubes[i].create(25 + randomFloat() * 50.0);
            cubeTransforms[i] = glm::mat4(1.0);
            cubeTransforms[i] = glm::translate(
                cubeTransforms[i],
                glm::vec3(
                    (spread / 2) - randomFloat() * spread,
                    (spread / 2) - randomFloat() * spread,
                    (spread / 2) - randomFloat() * spread));

            cubeDiffuseTextureIds[i] = textureIds[i % textureIds.size()];
        }
    }

    void render()
    {
        shader.use();
        atlasManager.bindAll(shader);
        shader.setUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());

        for (unsigned int i = 0; i < numCubes; i++)
        {
            shader.setUniform("diffuseTextureId", cubeDiffuseTextureIds[i]);
            shader.setUniform("modelMatrix", cubeTransforms[i]);
            cubes[i].draw();
        }
    }

    void destroy()
    {
    }
};