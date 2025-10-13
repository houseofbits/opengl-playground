#include "MainRenderSystem.h"
#include "../Components/SkyComponent.h"
#include <GL/glew.h>
#include "../../Application/Events/SystemEvent.h"
#include "../Resources/DeferredRenderTargetResource.h"
#include "../../../Renderer/Queue/RenderQueueBuilder.h"
#include "../Resources/RenderQueueResource.h"

MainRenderSystem::MainRenderSystem() : EntitySystem(),
                                       m_isEnabled(true),
                                       m_viewportWidth(1024),
                                       m_viewportHeight(768){
}

void MainRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleSystemEvent);
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleEditorUIEvent);
    eventManager.registerEventReceiver(this, &MainRenderSystem::handleCameraActivationEvent);
}

void MainRenderSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::WINDOW_RESIZED || event.eventType == SystemEvent::WINDOW_CREATED) {
        m_viewportWidth = event.window->viewportWidth;
        m_viewportHeight = event.window->viewportHeight;
    }
}

void MainRenderSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    resourceManager.request(m_mainRenderQueue, "mainRenderQueue");
}

void MainRenderSystem::process(EventManager &eventManager) {
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    Camera *camera = m_activeCameraHelper.find(*m_EntityContext);
    if (camera == nullptr) {
        return;
    }

    if (m_isEnabled && m_mainRenderQueue.isReady()) {
        m_mainRenderQueue().render(*camera);
    }
}

void MainRenderSystem::handleEditorUIEvent(const EditorUIEvent &event) {

}
