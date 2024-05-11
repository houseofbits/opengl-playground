#pragma once

#include <GL/glew.h>
#include "BaseRenderTest.h"
#include "../../Renderer/Texture.h"
#include "../../PostProcessRenderer/PostProcessRenderer.h"
#include "../../Renderer/TextureLoader.h"
#include "../../Renderer/UniformBuffer.h"
#include "../../Helper/QuadTree.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

class TestTextureAtlasDepth : public BaseRenderTest
{
private:
    PostProcessRenderer renderer;
    TextureAtlas atlas;
    TextureAtlasManager atlasManager;
    Camera camera;
    Shader shader;
    Cube cube;
    glm::mat4 cubeTransform;

public:
    void setup(Window &window)
    {
        message("Test TextureAtlas - depth buffer render target");

        renderer.init(glm::vec4(-1, -1, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageDepth.frag");

        atlasManager.init();

        renderer.textureId = atlasManager.getTextureId(TextureAtlasManager::ATLAS_SHADOW_DEPTH);

        camera.setPosition(glm::vec3(50, 200, -500))
            .setAngles(glm::radians(-10.0f), glm::radians(-25.0f))
            .resize(window.viewportWidth, window.viewportHeight);

        shader.loadProgram("resources/test/shaders/basic.vert", "resources/test/shaders/textureAtlasDepth.frag");

        cube.create(300);
        cubeTransform = glm::mat4(1.0);
        cubeTransform = glm::rotate(cubeTransform, glm::radians(30.0f), glm::vec3(0, 1, 0));
    }

    void render(Camera&)
    {
        renderer.draw();

        atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).bindRenderTarget();
        atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).qtOccupancy.clear();

        int size = 1024;
        for (unsigned int i = 0; i < 15; i++)
        {
            int index = atlasManager.occupyRegion(TextureAtlasManager::ATLAS_SHADOW_DEPTH, size);
            if (index > 0)
            {
                glm::uvec4 viewport = atlasManager.getRegionRect(TextureAtlasManager::ATLAS_SHADOW_DEPTH, index);
                glViewport(viewport.x, viewport.y, viewport.z, viewport.w);

                shader.use();
                shader.setUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());
                shader.setUniform("modelMatrix", cubeTransform);
                cube.draw();

                if (size > 128)
                {
                    size = size / 2;
                }
            }
        }

        atlasManager.getAtlas(TextureAtlasManager::ATLAS_SHADOW_DEPTH).unbindRenderTarget();
    }

    void destroy()
    {
    }
};