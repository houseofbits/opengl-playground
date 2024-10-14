#include "CameraControlInputBehaviour.h"
#include "../Components/CameraComponent.h"
#include "../../../Core/Helper/Time.h"

CameraControlInputBehaviour::CameraControlInputBehaviour() : EntityBehaviour() {

}

void CameraControlInputBehaviour::initialize(ResourceManager &) {

}

void CameraControlInputBehaviour::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &CameraControlInputBehaviour::handleInputEvent);
}

std::string CameraControlInputBehaviour::getDescription() {
    return "Camera controller for editor use (Ctrl + mouse-look and move)";
}

void CameraControlInputBehaviour::handleInputEvent(const InputEvent& event) {
    if (!event.modKeyCtrl) {
        return;
    }

    auto *cameraComponent = m_Entity->getComponent<CameraComponent>();
    if (cameraComponent == nullptr || !cameraComponent->m_isActive) {
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
