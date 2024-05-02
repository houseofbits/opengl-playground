#pragma once

#include <GL/glew.h>
#include "BaseRenderTest.h"
#include "../../Renderer/Texture.h"
#include "../../PostProcessRenderer/PostProcessRenderer.h"
#include "../../Renderer/TextureLoader.h"

class TestTexture : public BaseRenderTest
{
private:
    PostProcessRenderer renderer;
    Texture texture1;

public:
    void setup(Window &window)
    {
        message("Test Texture and TextureLoader");

        renderer.init(glm::vec4(0, 0, 1, 1), "resources/shaders/2dimage.vert", "resources/shaders/2dimageColor.frag");

        texture1 = TextureLoader::load("resources/textures/concrete.jpg");

        renderer.textureId = texture1.textureId;
    }

    void render()
    {
        renderer.draw();
    }

    void destroy()
    {
    }
};