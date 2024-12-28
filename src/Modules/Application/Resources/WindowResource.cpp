#include "WindowResource.h"

WindowResource::WindowResource() : Resource(), m_Window(nullptr) {
}

Resource::Status WindowResource::fetchData(ResourceManager &resource_manager) {
    return Resource::STATUS_DATA_READY;
}

Resource::Status WindowResource::build() {
    return Resource::STATUS_READY;
}

void WindowResource::destroy() {
}

void WindowResource::create(EventManager &eventManager) {
    m_Window = new Window();
    m_Window->create(eventManager);
}

void WindowResource::swapBuffers() const {
    if (m_Window != nullptr) {
        return m_Window->doubleBuffer();
    }
}

glm::ivec2 WindowResource::getViewportSize() const {
    if (m_Window == nullptr) {
        return glm::ivec2(0, 0);
    }

    return glm::ivec2(m_Window->viewportWidth, m_Window->viewportHeight);
}
