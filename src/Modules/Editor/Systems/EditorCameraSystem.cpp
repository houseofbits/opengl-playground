#include "EditorCameraSystem.h"
#include "../../../Core/Helper/Time.h"
#include "../../Application/Events/InputEvent.h"
#include "../../Application/Events/SystemEvent.h"

EditorCameraSystem::EditorCameraSystem() : EntitySystem(), m_viewportSize(0, 0) {
    m_registry = useEntityUniqueComponentRegistry<EditorCameraComponent>();
}

void EditorCameraSystem::process(EventManager &) {
}

void EditorCameraSystem::initialize(ResourceManager &, EventManager&) {
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

    for (const auto [id, cameraComponent]: m_registry->container()) {
        if (!cameraComponent->m_isActive) {
            continue;
        }

        if (event.type == InputEvent::MOUSEWHEEL && cameraComponent->isOrthographic()) {
            float scale = cameraComponent->m_orthographicScale += event.mouseWheel.y * 0.1;
            if (scale < 0.2) {
                scale = 0.2;
            }
            cameraComponent->setOrthographicScale(scale);
        }

        if (event.type == InputEvent::MOUSEMOVE && event.mouseButtonLeft) {
            const float lookSpeed = 0.15;
            cameraComponent->rotateView(-event.mouseMotion * lookSpeed * Time::frameTime);
        }

        float moveSpeed = 3;
        if (cameraComponent->isOrthographic()) {
            moveSpeed = cameraComponent->m_orthographicScale * 0.1;
        }

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
}

void EditorCameraSystem::handleCameraActivationEvent(const CameraActivationEvent &event) {
    for (const auto [id, component]: m_registry->container()) {
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
