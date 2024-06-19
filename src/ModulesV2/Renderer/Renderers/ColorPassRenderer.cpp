#include "ColorPassRenderer.h"
#include "../Systems/RendererSystem.h"

ColorPassRenderer::ColorPassRenderer(RendererSystem *system) : m_RenderSystem(system),
                                                               m_Shader(),
                                                               m_Camera() {
}

void ColorPassRenderer::initialize() {
    m_Shader.loadProgram(
            "data/shaders/basic.vert",
            "data/shaders/basic.frag");

    m_Camera.setPosition(glm::vec3(20, 15, -50))
            .setView(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0))
            .setFieldOfView(90)
            .setViewportSize(1024, 768);
}

void ColorPassRenderer::beginRender(Camera &camera) {
    m_Shader.use();
    camera.bind(m_Shader);
}

void ColorPassRenderer::render(ColorPassFrameData &frame) {
    for (const auto &elem: frame.m_Elements) {
        if (elem.second.material != nullptr) {
            m_Shader.setUniform("diffuseSampler", elem.second.material->m_Diffuse().handleId);
        }
        elem.second.mesh->m_Mesh().render();
    }
}
