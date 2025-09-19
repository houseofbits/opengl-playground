#include "WireframeRenderer.h"
#include "../Camera/Camera.h"
#include "../Model/WireModel.h"
#include "Shader.h"
#include <GL/glew.h>

WireframeRenderer::WireframeRenderer() : m_shader(), m_wireCube(), m_wireSphere(), m_pCamera(nullptr) {
}

void WireframeRenderer::initialize() {
    const std::string vertexShader = "#version 410\n"
            "layout (location=0) in vec3 VertexPosition;\n"
            "layout (location=1) in vec4 VertexColor;\n"
            "uniform mat4 viewProjectionMatrix;\n"
            "uniform mat4 modelMatrix;\n"
            "out vec4 vsColor;\n"
            "void main() {\n"
            "vsColor = VertexColor;\n"
            "gl_Position = viewProjectionMatrix * modelMatrix * vec4(VertexPosition,1.0);\n"
            "}";

    const std::string fragmentShader = "#version 410\n"
            "layout (location=0) out vec4 FragColor;\n"
            "in vec4 vsColor;\n"
            "uniform vec4 color;\n"
            "void main() {\n"
            "FragColor = vsColor * color;\n"
            "}";

    m_shader.loadFromString(GL_VERTEX_SHADER, vertexShader);
    m_shader.loadFromString(GL_FRAGMENT_SHADER, fragmentShader);
    m_shader.compileAndLink();

    m_wireCube.create({1, 1, 1, 1});
    m_wireSphere.create({1, 1, 1, 1});
}

void WireframeRenderer::render(const glm::mat4 &transform, const WireModel &model,
                               const glm::vec4 color) {
    assert(m_pCamera != nullptr);

    m_shader.use();
    m_shader.setUniform("modelMatrix", transform);
    m_shader.setUniform("color", color);
    m_pCamera->bind(m_shader);
    model.draw();
}

void WireframeRenderer::renderCube(const glm::mat4 &transform, const glm::vec4 color) {
    render(transform, m_wireCube, color);
}

void WireframeRenderer::renderSphere(const glm::mat4 &transform, const glm::vec4 color) {
    render(transform, m_wireSphere, color);
}
