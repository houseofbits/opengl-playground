#include "DebugInformationRenderer.h"
#include "../RenderManager.h"

DebugInformationRenderer::DebugInformationRenderer(RenderManager *manager) : BaseRenderer(manager), lightWireframeShader(),
                                                                             lineMesh() {
}

void DebugInformationRenderer::init() {
    lightWireframeShader.loadProgram(
            "resources/shaders/lines/lines.vert",
            "resources/shaders/lines/lines.frag",
            "resources/shaders/lines/lines.geom");

    lineMesh.setColor(glm::vec4(1, 1, 0, 1));
    lineMesh.addVertex(glm::vec3(-12.3296, 0, 11.9947));
    lineMesh.addVertex(glm::vec3(-12.3296, 2.08944, 11.9947));
    lineMesh.build();
}

void DebugInformationRenderer::renderLightCones(Scene &scene, Camera &camera) {
    lightWireframeShader.use();
    camera.bind(lightWireframeShader);

    for (int i = 0; i < this->renderManager->lightsUniformBuffer.getNumActiveLights(); ++i) {
        glm::mat4 invProj = glm::inverse(this->renderManager->lightsUniformBuffer.get(i).projectionViewMatrix);
        lightWireframeShader.setUniform("inverseProjectionViewMatrix", invProj);
        lineMesh.draw();
    }
}
