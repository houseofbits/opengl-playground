#pragma once

#include "../../CoreV2/Events/EventManager.h"
#include "../../Window.h"
#include "BaseRenderTest.h"
#include "TestMipMapping.h"
#include "TestNormalMapping.h"
#include "TestTangentSpaceCalculation.h"
#include "TestTexture.h"
#include "TestTextureAtlas.h"
#include "TestTextureAtlasDepth.h"
#include "TestTextureAtlasShader.h"

class RenderTests
{
public:
    static void run()
    {
        TestTexture textureTest;
        TestTextureAtlas atlasTest;
        TestTextureAtlasShader atlasShaderTest;
        TestTextureAtlasDepth atlasDepthTest;
        TestTangentSpaceCalculation tangentSpaceCalculation;
        TestNormalMapping testNormalMapping;
        TestMipMapping testMipMapping;

        // runSingleTest(&textureTest);
        // runSingleTest(&atlasTest);
//         runSingleTest(&atlasShaderTest);
//        runSingleTest(&atlasDepthTest);
//        runSingleTest(&tangentSpaceCalculation);
//        runSingleTest(&testNormalMapping);
        runSingleTest(&testMipMapping);
    }

    static void runSingleTest(BaseRenderTest *test)
    {
        EventManager eventManager;
        Window window(&eventManager);
        Camera camera;
//        camera.registerEventHandlers(&eventManager);
        window.create();

        test->setup(window);

        glEnable(GL_DEPTH_TEST);

        while (true)
        {
            if (!window.pollEvents() || !eventManager.processEvents())
            {
                break;
            }

            glViewport(0, 0, window.viewportWidth, window.viewportHeight);
            glClearColor(0.f, 1.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            test->render(camera);

            window.doubleBuffer();
        }

        test->destroy();

        window.destroy();
    }
};