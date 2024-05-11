#pragma once

#include <GL/glew.h>
#include "BaseRenderTest.h"
#include "../../Renderer/Texture.h"
#include "../../PostProcessRenderer/PostProcessRenderer.h"
#include "../../Renderer/TextureLoader.h"
#include "../../Renderer/UniformBuffer.h"
#include "../../Helper/QuadTree.h"

class TestTextureAtlas : public BaseRenderTest
{
private:
    PostProcessRenderer renderer;
    TextureAtlas atlas;
    TextureAtlasManager atlasManager;

public:
    void setup(Window &window)
    {
        message("Test TextureAtlas - texture loading into atlas");

        renderer.init(glm::vec4(-1, -1, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageColor.frag");

        atlasManager.init();

        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-512.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-512.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-256.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-256.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-128.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-128.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-128.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-128.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-128.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-512.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-256.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-256.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t-256.png", TextureAtlasManager::ATLAS_DIFFUSE);
        atlasManager.loadTextureIntoAtlas("resources/test/textures/t2-256.png", TextureAtlasManager::ATLAS_DIFFUSE);

        renderer.textureId = atlasManager.getTextureId(TextureAtlasManager::ATLAS_DIFFUSE);
    }

    void render(Camera&)
    {
        renderer.draw();
    }

    void destroy()
    {
    }
};