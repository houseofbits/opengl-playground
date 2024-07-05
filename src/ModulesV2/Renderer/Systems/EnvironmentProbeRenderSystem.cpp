#include "EnvironmentProbeRenderSystem.h"
#include "../Components/CameraComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/StaticMeshComponent.h"
#include <GL/glew.h>

EnvironmentProbeRenderSystem::EnvironmentProbeRenderSystem() : EntitySystem(),
                                                               m_ShaderProgram(),
                                                               m_LightsBuffer(),
                                                               m_viewportWidth(1024),
                                                               m_viewportHeight(768) {
    usesComponent<StaticMeshComponent>();
    usesComponent<LightComponent>();
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<EnvironmentProbeComponent>();
}

void EnvironmentProbeRenderSystem::initialize(ResourceManager *resourceManager) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    resourceManager->request(m_ShaderProgram, "data/shaders/lighting|.vert|.frag|.geom");
    resourceManager->request(m_LightsBuffer, "SpotLightStorageBuffer");
}

void EnvironmentProbeRenderSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &EnvironmentProbeRenderSystem::handleWindowEvent);
}

bool EnvironmentProbeRenderSystem::handleWindowEvent(WindowEvent *const event) {
    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE) {
        m_viewportWidth = event->window->viewportWidth;
        m_viewportHeight = event->window->viewportHeight;
    }

    return true;
}

void EnvironmentProbeRenderSystem::process() {

    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    m_ShaderProgram().use();
}

TransformComponent *EnvironmentProbeRenderSystem::findTransform(Identity &entityId) {
    return getComponent<TransformComponent>(entityId.id());
}
