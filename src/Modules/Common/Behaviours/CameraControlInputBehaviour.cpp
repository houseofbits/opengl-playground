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

void CameraControlInputBehaviour::handleInputEvent(const InputEvent *const event) {
    if (!event->modKeyCtrl) {
        return;
    }

    auto *cameraComponent = m_Entity->getComponent<CameraComponent>();
    if (cameraComponent == nullptr || !cameraComponent->m_isActive) {
        return;
    }

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft) {
        float lookSpeed = 0.15;
        cameraComponent->rotateView(-event->mouseMotion * lookSpeed * Time::frameTime);
    }

    Camera *camera = &cameraComponent->m_Camera;
    glm::vec3 position = camera->getPosition();
    auto tbn = cameraComponent->calculateTBN(camera->getViewDirection());
    float moveSpeed = 3;

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonRight) {
        position += tbn.up * (moveSpeed * event->mouseMotion.y * Time::frameTime);
        position -= tbn.right * (moveSpeed * event->mouseMotion.x * Time::frameTime);
        camera->setPosition(position);
        camera->setView(tbn.view, tbn.up);
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 26) {
        position += tbn.view * (moveSpeed * Time::frameTime);
        camera->setPosition(position);
        camera->setView(tbn.view, tbn.up);
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 22) {
        position -= tbn.view * (moveSpeed * Time::frameTime);
        camera->setPosition(position);
        camera->setView(tbn.view, tbn.up);
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 4) {
        position -= tbn.right * (moveSpeed * Time::frameTime);
        camera->setPosition(position);
        camera->setView(tbn.view, tbn.up);
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 7) {
        position += tbn.right * (moveSpeed * Time::frameTime);
        camera->setPosition(position);
        camera->setView(tbn.view, tbn.up);
    }
}
