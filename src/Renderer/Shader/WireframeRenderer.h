#pragma once
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include "../Model/WireCube.h"
#include "../Model/WireSphere.h"

class Camera;
class WireModel;

class WireframeRenderer {
public:
    WireframeRenderer();

    void initialize();

    void render(const glm::mat4 &transform, const WireModel &model, glm::vec4 color = {1, 1, 1, 1});

    void renderCube(const glm::mat4 &transform, glm::vec4 color);

    void renderSphere(const glm::mat4 &transform, glm::vec4 color);

    void setCamera(Camera& camera) {
        m_pCamera = &camera;
    }

private:
    Camera *m_pCamera;
    Shader m_shader;
    WireCube m_wireCube;
    WireSphere m_wireSphere;
};
