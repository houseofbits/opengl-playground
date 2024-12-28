#include "ApplicationEventsSystem.h"

ApplicationEventsSystem::ApplicationEventsSystem() : EntitySystem(), isRunning(true),
                                                     m_WindowResource() {
}

void ApplicationEventsSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    resourceManager.request(m_WindowResource, "window");
}

void ApplicationEventsSystem::process(EventManager &) {
    if (!m_WindowResource().pollEvents()) {
        isRunning = false;
    }
}

void ApplicationEventsSystem::registerEventHandlers(EventManager &eventManager) {
}
