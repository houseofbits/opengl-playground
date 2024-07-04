#include "ColorPassRenderer.h"
#include "../Systems/RendererSystem.h"

ColorPassRenderer::ColorPassRenderer(RendererSystem *system) : m_RenderSystem(system),
                                                               m_ShaderProgram() {
}

void ColorPassRenderer::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_ShaderProgram, "data/shaders/lighting|.vert|.frag|.geom");
}

void ColorPassRenderer::beginRender(Camera &camera) {
    m_ShaderProgram().use();
    camera.bind(m_ShaderProgram());
}

void ColorPassRenderer::render(ColorPassFrameData &frame) {
    for (const auto &elem: frame.m_Elements) {
        elem.second.mesh->m_Material().bind(m_ShaderProgram());
        m_ShaderProgram().setUniform("modelMatrix", elem.second.transform->getModelMatrix());
        frame.m_LightBuffer.bind(m_ShaderProgram());
        frame.m_ProjectorBuffer.bind();
        frame.m_EnvironmentProbeBuffer.bind(m_ShaderProgram());
        elem.second.mesh->m_Mesh().render();
    }
}
