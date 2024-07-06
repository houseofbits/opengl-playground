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
//    resourceManager->request(m_ShaderProgram, "data/shaders/lighting|.vert|.frag|.geom");
//    resourceManager->request(m_LightsBuffer, "SpotLightStorageBuffer");
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

}

TransformComponent *EnvironmentProbeRenderSystem::findTransform(Identity &entityId) {
    return getComponent<TransformComponent>(entityId.id());
}
