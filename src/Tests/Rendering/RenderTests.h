#pragma once

#include "../../Events/EventManager.h"
#include "../../Window.h"
#include "BaseRenderTest.h"
#include "TestTexture.h"
#include "TestTextureAtlas.h"
#include "TestTextureAtlasShader.h"
#include "TestTextureAtlasDepth.h"

class RenderTests
{
public:
    static void run()
    {
        TestTexture textureTest;
        TestTextureAtlas atlasTest;
        TestTextureAtlasShader atlasShaderTest;
        TestTextureAtlasDepth atlasDepthTest;

        // runSingleTest(&textureTest);
        // runSingleTest(&atlasTest);
        // runSingleTest(&atlasShaderTest);
        runSingleTest(&atlasDepthTest);
    }

    static void runSingleTest(BaseRenderTest *test)
    {
        EventManager eventManager;
        Window window(&eventManager);
        window.create();

        test->setup(window);

        glEnable(GL_DEPTH_TEST);

        while (true)
        {
            if (!window.pollEvents())
            {
                break;
            }

            glViewport(0, 0, window.viewportWidth, window.viewportHeight);
            glClearColor(1.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            test->render();

            window.doubleBuffer();
        }

        test->destroy();

        window.destroy();
    }
};