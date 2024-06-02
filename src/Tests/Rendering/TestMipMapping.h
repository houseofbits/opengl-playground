#pragma once

#include "../../Renderer/Model/Cube.h"
#include "../../Renderer/Texture/TextureLoader.h"
#include "../../Renderer/TextureAtlas/TextureAtlasManager.h"
#include "BaseRenderTest.h"
#include <glm/gtx/transform.hpp>

class TestMipMapping : public BaseRenderTest {
private:
    TextureAtlasManager atlasManager;
    Shader shader;
    Cube cube;
    glm::mat4 cubeTransform;
    unsigned int textureId;

public:
    void setup(Window &window) override {
        message("Test mip-mapping");

        atlasManager.init(2);

        shader.loadProgram("resources/test/shaders/basic.vert", "resources/test/shaders/textureAtlasMapping.frag");

        textureId = atlasManager.loadTextureIntoAtlas("resources/test/textures/t-256.png", TextureAtlasManager::ATLAS_DIFFUSE);

        cube.create(1.0);
        cubeTransform = glm::mat4(1.0);
        cubeTransform = glm::scale(
                cubeTransform,
                glm::vec3(500, 100, 500));
    }

    void
    render(Camera &camera) override {

        shader.use();
        shader.setUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());
        atlasManager.bindAll(shader);
        shader.setUniform("diffuseTextureId", textureId);
        shader.setUniform("modelMatrix", cubeTransform);
        cube.draw();
    }

    void destroy() override {
    }
};
