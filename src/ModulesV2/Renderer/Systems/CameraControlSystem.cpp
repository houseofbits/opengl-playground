#include "CameraControlSystem.h"
#include "../../../CoreV2/Events/InputEvent.h"
#include "../../../CoreV2/Events/WindowEvent.h"
#include "../../../Helper/Time.h"

CameraControlSystem::CameraControlSystem() : m_cameraComponents() {
}

void CameraControlSystem::process() {
}

void CameraControlSystem::initialize(ResourceManager*) {
}

void CameraControlSystem::registerComponent(Component *comp) {
    if (isOfType<CameraComponent>(comp)) {
        m_cameraComponents[comp->m_Id()] = dynamic_cast<CameraComponent *>(comp);
    }
}

void CameraControlSystem::unregisterComponent(Component *comp) {
}

void CameraControlSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CameraControlSystem::handleWindowEvent);
    eventManager->registerEventReceiver(this, &CameraControlSystem::handleInputEvent);
}

bool CameraControlSystem::handleWindowEvent(WindowEvent *const event) {
    Camera *camera = findActiveCamera();
    if (!camera) {
        return true;
    }

    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE) {
        camera->setViewportSize(event->window->viewportWidth, event->window->viewportHeight);
    }

    return true;
}

bool CameraControlSystem::handleInputEvent(InputEvent *const event) {

    if(!event->modKeyCtrl) {
        return true;
    }

    Camera *camera = findActiveCamera();
    if (!camera) {
        return true;
    }

    float moveSpeed = 4;
    float lookSpeed = 0.15;

    glm::vec3 position = camera->getPosition();
    TBN tbn = calculateTBN(camera->getViewDirection());

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft) {
        glm::vec3 rightHorizontal = glm::normalize(glm::cross(tbn.view, glm::vec3(0, 1, 0)));
        glm::vec3 upVertical = glm::normalize(glm::cross(tbn.view, rightHorizontal));

        float rate = lookSpeed * Time::frameTime * -event->mouseMotion.x;
        glm::vec3 viewChange = rightHorizontal * rate;
        tbn = calculateTBN(tbn.view + viewChange);

        rate = lookSpeed * Time::frameTime * -event->mouseMotion.y;
        viewChange = upVertical * rate;
        tbn = calculateTBN(tbn.view + viewChange);

        camera->setView(tbn.view, tbn.up);
    }

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

    if (event->type == InputEvent::KEYDOWN && event->keyCode == 58)// F1
    {
        std::cout << "Camera - position:"
                  << (position.x) << "," << (position.y) << "," << (position.z)
                  << " direction:"
                  << (tbn.view.x) << "," << (tbn.view.y) << "," << (tbn.view.z)
                  << std::endl;
    }
    return true;
}

Camera *CameraControlSystem::findActiveCamera() {
    for (const auto &camera: m_cameraComponents) {
        if (camera.second->m_isActive) {
            return &camera.second->m_Camera;
        }
    }

    return nullptr;
}

CameraControlSystem::TBN CameraControlSystem::calculateTBN(glm::vec3 viewDirection) {
    CameraControlSystem::TBN tbn{};

    tbn.view = glm::normalize(viewDirection);
    tbn.right = glm::normalize(glm::cross(tbn.view, glm::vec3(0, 1, 0)));
    tbn.up = glm::normalize(glm::cross(tbn.right, viewDirection));

    return tbn;
}
