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
        if (elem.second.material != nullptr) {
            m_ShaderProgram().setUniform("diffuseSampler", elem.second.material->m_Diffuse().m_handleId);
            m_ShaderProgram().setUniform("normalSampler", elem.second.material->m_Normal().m_handleId);
        }
        m_ShaderProgram().setUniform("modelMatrix", elem.second.transform->getModelMatrix());
        frame.m_LightBuffer.bind(m_ShaderProgram());
        frame.m_ProjectorBuffer.bind();
        elem.second.mesh->m_Mesh().render();
    }
}
