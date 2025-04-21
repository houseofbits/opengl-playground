#include "OverlayRenderSystem.h"
#include <GL/glew.h>
#include "../Resources/DeferredRenderTargetResource.h"

OverlayRenderSystem::OverlayRenderSystem() : EntitySystem() {
    // m_lights = useEntityUniqueComponentRegistry<LightComponent>();
}

void OverlayRenderSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    resourceManager.request(m_ShaderProgram,
                            "data/shaders/deferred/|quad.vert|texturedQuad.frag");

    // resourceManager.request(m_computeTestShader, "data/shaders/compute/test.cs");

    // resourceManager.request(m_computeTestShader, "data/shaders/compute/h-blur.cs");

    resourceManager.request(m_textureResource, "resources/textures/checker-map.png");
    resourceManager.request(m_crosshairTextureResource, "data/textures/crosshair198.png");
    resourceManager.request(m_deferredRenderTarget, "deferredRenderTarget");
}

void OverlayRenderSystem::process(EventManager &eventManager) {
    const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;

    if (!m_textureResource().isReady()) {
        return;
    }

    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
    }
    // if (texture == 0) {
    //     glGenTextures(1, &texture);
    //     // glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, texture);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     glTextureStorage2D(texture, 1, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA,
    //                  GL_FLOAT, nullptr);
    //
    //
    //     auto handle = glGetTextureHandleARB(texture);
    //     glMakeTextureHandleResidentARB(handle);
    // }
    //
    // unsigned int shadowTextureId = 0;
    // for (auto &[id, light] : m_lights->container()) {
    //     if(light->m_isEnabled && !light->m_ShadowMaps.empty()) {
    //         shadowTextureId = light->m_ShadowMaps.front()->get().m_textureRenderTarget.textureId;
    //         break;
    //     }
    // }

    // if (m_computeTestShader.isReady()) {
    //     m_computeTestShader().use();
    //     // m_textureResource().bindImageTexture(0);
    //     glBindImageTexture(0, shadowTextureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    //
    //     // glBindImageTexture(1, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    //
    //     m_computeTestShader().setUniform("blurDirection", glm::vec2(1,0));
    //     m_computeTestShader().dispatchCompute(2048, 2048);
    //
    //     m_computeTestShader().setUniform("blurDirection", glm::vec2(0,1));
    //     m_computeTestShader().dispatchCompute(2048, 2048);
    // }


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    m_ShaderProgram().use();

    m_ShaderProgram().setUniform("screenTexture", m_deferredRenderTarget().getHandle());
    m_ShaderProgram().setUniform("crosshairTexture", m_crosshairTextureResource().getHandle());

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glEnable(GL_DEPTH_TEST);
}

void OverlayRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &OverlayRenderSystem::handleSystemEvent);
    eventManager.registerEventReceiver(this, &OverlayRenderSystem::handleEditorUIEvent);
}

void OverlayRenderSystem::handleSystemEvent(const SystemEvent &event) {
}

void OverlayRenderSystem::handleEditorUIEvent(const EditorUIEvent &event) {
}
