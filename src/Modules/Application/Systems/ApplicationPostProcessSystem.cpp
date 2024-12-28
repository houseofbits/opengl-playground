#include "ApplicationPostProcessSystem.h"

ApplicationPostProcessSystem::ApplicationPostProcessSystem() : EntitySystem(), m_WindowResource() {
}

void ApplicationPostProcessSystem::initialize(ResourceManager &resourceManager, EventManager &eventManager) {
    resourceManager.request(m_WindowResource, "window");
}

void ApplicationPostProcessSystem::process(EventManager &) {
    m_WindowResource().swapBuffers();
}

void ApplicationPostProcessSystem::registerEventHandlers(EventManager &eventManager) {
}
