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
    m_Window = new Window(&eventManager);
    m_Window->create();
}

bool WindowResource::pollEvents() const {
    if (m_Window != nullptr) {
        return m_Window->pollEvents();
    }

    return true;
}

void WindowResource::swapBuffers() const {
    if (m_Window != nullptr) {
        return m_Window->doubleBuffer();
    }
}
