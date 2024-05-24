#include "CameraController.h"
#include "../../Events/EventManager.h"
#include "../../Events/InputEvent.h"
#include "../../Events/WindowEvent.h"
#include "../../Helper/Time.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

CameraController::CameraController() : camera(nullptr), verticalAngle(0), horizontalAngle(0), position(0.0f),
                                       direction(0.0f), up(0.0f), right(0.0f) {
    calculateTBN();
}

void CameraController::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CameraController::handleWindowEvent);
    eventManager->registerEventReceiver(this, &CameraController::handleInputEvent);
}

bool CameraController::handleWindowEvent(WindowEvent *const event) {
    assert(camera != nullptr);

    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE) {
        camera->resize(event->window->viewportWidth, event->window->viewportHeight);
    }

    return true;
}

bool CameraController::handleInputEvent(InputEvent *const event) {
    assert(camera != nullptr);

    float moveSpeed = 4;
    float lookSpeed = 0.1;

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft) {
        horizontalAngle += lookSpeed * Time::frameTime * event->mouseMotion.x;
        verticalAngle += lookSpeed * Time::frameTime * event->mouseMotion.y;
        updateCamera();
    }

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonRight) {
        position += up * (moveSpeed * event->mouseMotion.y * Time::frameTime);
        position -= right * (moveSpeed * event->mouseMotion.x * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 26) {
        position += direction * (moveSpeed * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 22) {
        position -= direction * (moveSpeed * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 4) {
        position -= right * (moveSpeed * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 7) {
        position += right * (moveSpeed * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYDOWN && event->keyCode == 58)// F1
    {
        std::cout << "Camera - vertical angle: "
                  << (glm::degrees(verticalAngle))
                  << " horizontal angle: "
                  << (glm::degrees(horizontalAngle))
                  << " position:"
                  << (position.x) << "," << (position.y) << "," << (position.z)
                  << " direction:"
                  << (direction.x) << "," << (direction.y) << "," << (direction.z)
                  << std::endl;
    }
    return true;
}

void CameraController::calculateTBN() {
    direction = glm::vec3(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle));

    direction = glm::normalize(direction);

    right = glm::vec3(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f));

    right = glm::normalize(right);

    up = glm::cross(right, direction);
    up = glm::normalize(up);
}

void CameraController::updateCamera() {
    calculateTBN();

    camera->setPosition(position);
    camera->setView(direction, up);
}

void CameraController::attachCamera(Camera *cam) {
    camera = cam;

    //NO NO NO NO THIS IS NOT RIGHT
    //No need for angles at all, just rotate TBN
    verticalAngle = cam->verticalAngle;
    horizontalAngle = cam->horizontalAngle;
    position = camera->getPosition();

    calculateTBN();
}