#pragma once

#include "../../Renderer/Cube.h"
#include "../../Renderer/TextureAtlasManager.h"
#include "BaseRenderTest.h"
#include <glm/gtx/transform.hpp>

class TestNormalMapping : public BaseRenderTest {
private:
    static const int numCubes = 30;

    TextureAtlasManager atlasManager{};
    Shader shader;
    Cube cubes[numCubes]{};
    glm::mat4 cubeTransforms[numCubes]{};
    unsigned int normalMapId = {0};
    unsigned int diffuseMapId = {0};

            static float randomFloat() {
        return (float) (rand()) / (float) (RAND_MAX);
    }

public:
    void setup(Window &window) override {
        message("Test normal map rendering");

        atlasManager.init();

        shader.loadProgram("resources/test/shaders/normals.vert", "resources/test/shaders/normals.frag", "resources/test/shaders/normals.geom");

        normalMapId = atlasManager.loadTextureIntoAtlas("resources/test/textures/normal-map.png", TextureAtlasManager::ATLAS_NORMALS);
        diffuseMapId = atlasManager.loadTextureIntoAtlas("resources/test/textures/t-512.png", TextureAtlasManager::ATLAS_DIFFUSE);

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
        }
    }

    void render(Camera &camera) override {

        shader.use();
        atlasManager.bindAll(shader);
        shader.setUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());

        shader.setUniform("diffuseTextureId", diffuseMapId);
        shader.setUniform("normalTextureId", normalMapId);

        for (unsigned int i = 0; i < numCubes; i++)
        {

            shader.setUniform("modelMatrix", cubeTransforms[i]);
            cubes[i].draw();
        }
    }

    void destroy() override {
    }
};
