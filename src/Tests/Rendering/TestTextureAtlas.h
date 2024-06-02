#pragma once

#include "../../Helper/QuadTree.h"
#include "../../PostProcessRenderer/PostProcessRenderer.h"
#include "../../Renderer/Shader/UniformBuffer.h"
#include "../../Renderer/Texture/Texture.h"
#include "../../Renderer/Texture/TextureLoader.h"
#include "BaseRenderTest.h"
#include <GL/glew.h>

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

        atlasManager.init(2);

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