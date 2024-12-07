#include "EditorCameraSystem.h"
#include "../Components/EditorCameraComponent.h"
#include "../../../Core/Helper/Time.h"

EditorCameraSystem::EditorCameraSystem() : EntitySystem(), m_viewportSize(0, 0) {
    usesComponent<EditorCameraComponent>();
}

void EditorCameraSystem::process(EventManager &) {

}

void EditorCameraSystem::initialize(ResourceManager &) {

}

void EditorCameraSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &EditorCameraSystem::handleInputEvent);
    eventManager.registerEventReceiver(this, &EditorCameraSystem::handleSystemEvent);
    eventManager.registerEventReceiver(this, &EditorCameraSystem::handleCameraActivationEvent);
}

void EditorCameraSystem::handleInputEvent(const InputEvent &event) {
    if (!event.modKeyCtrl) {
        return;
    }

    auto *cameraComponent = findComponent<EditorCameraComponent>([](EditorCameraComponent *c) {
        return c->m_isActive;
    });

    if (!cameraComponent) {
        return;
    }

    if (event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) {
        float lookSpeed = 0.15;
        cameraComponent->rotateView(-event.mouseMotion * lookSpeed * Time::frameTime);
    }

    float moveSpeed = 3;
    glm::vec3 direction(0.0);
    bool doMove = false;

    if (event.type == InputEvent::MOUSEMOVE && event.mouseButtonRight) {
        direction.x -= event.mouseMotion.x;
        direction.y += event.mouseMotion.y;
        doMove = true;
    }

    if (event.type == InputEvent::KEYPRESS && event.keyCode == 26) {
        direction.z += 1;
        doMove = true;
    }

    if (event.type == InputEvent::KEYPRESS && event.keyCode == 22) {
        direction.z -= 1;
        doMove = true;
    }

    if (event.type == InputEvent::KEYPRESS && event.keyCode == 4) {
        direction.x -= 1;
        doMove = true;
    }

    if (event.type == InputEvent::KEYPRESS && event.keyCode == 7) {
        direction.x += 1;
        doMove = true;
    }

    if (doMove) {
        cameraComponent->moveView(glm::normalize(direction) * moveSpeed * Time::frameTime);
    }
}

void EditorCameraSystem::handleCameraActivationEvent(const CameraActivationEvent &event) {
    for (const auto [id, component]: getComponentContainer<EditorCameraComponent>()) {
        component->setActive(false);

        if (id == event.m_cameraComponentId) {
            component->setActive(true);
            component->setViewportSize(m_viewportSize);
        }
    }
}

void EditorCameraSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::WINDOW_RESIZED || event.eventType == SystemEvent::WINDOW_CREATED) {
        m_viewportSize.x = event.window->viewportWidth;
        m_viewportSize.y = event.window->viewportHeight;
    }
}