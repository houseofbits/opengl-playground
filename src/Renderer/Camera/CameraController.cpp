#include "CameraController.h"
#include "../../CoreV2/Events/EventManager.h"
#include "../../CoreV2/Events/InputEvent.h"
#include "../../CoreV2/Events/WindowEvent.h"
#include "../../Helper/Time.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

CameraController::CameraController() : camera(nullptr), position(0.0f),
                                       viewDirection(0.0f), upDirection(0.0f), rightDirection(0.0f) {
    calculateTBN();
}

void CameraController::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CameraController::handleWindowEvent);
    eventManager->registerEventReceiver(this, &CameraController::handleInputEvent);
}

bool CameraController::handleWindowEvent(WindowEvent *const event) {
    assert(camera != nullptr);

    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE) {
        camera->setViewportSize(event->window->viewportWidth, event->window->viewportHeight);
    }

    return true;
}

bool CameraController::handleInputEvent(InputEvent *const event) {
    assert(camera != nullptr);

    float moveSpeed = 4;
    float lookSpeed = 0.15;

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft) {
        glm::vec3 rightHorizontal = glm::normalize(glm::cross(viewDirection, glm::vec3(0,1,0)));
        glm::vec3 upVertical = glm::normalize(glm::cross(viewDirection, rightHorizontal));

        float rate = lookSpeed * Time::frameTime * -event->mouseMotion.x;
        glm::vec3 viewChange = rightHorizontal * rate;
        viewDirection = viewDirection + viewChange;
        calculateTBN();

        rate = lookSpeed * Time::frameTime * -event->mouseMotion.y;
        viewChange = upVertical * rate;
        viewDirection = viewDirection + viewChange;
        calculateTBN();

        updateCamera();
    }

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonRight) {
        position += upDirection * (moveSpeed * event->mouseMotion.y * Time::frameTime);
        position -= rightDirection * (moveSpeed * event->mouseMotion.x * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 26) {
        position += viewDirection * (moveSpeed * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 22) {
        position -= viewDirection * (moveSpeed * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 4) {
        position -= rightDirection * (moveSpeed * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 7) {
        position += rightDirection * (moveSpeed * Time::frameTime);
        updateCamera();
    }

    if (event->type == InputEvent::KEYDOWN && event->keyCode == 58)// F1
    {
        std::cout << "Camera - position:"
                  << (position.x) << "," << (position.y) << "," << (position.z)
                  << " direction:"
                  << (viewDirection.x) << "," << (viewDirection.y) << "," << (viewDirection.z)
                  << std::endl;
    }
    return true;
}

void CameraController::calculateTBN() {
    viewDirection = glm::normalize(viewDirection);
    rightDirection = glm::normalize(glm::cross(viewDirection, glm::vec3(0,1,0)));
    upDirection = glm::normalize(glm::cross(rightDirection, viewDirection));
}

void CameraController::updateCamera() {
    camera->setPosition(position);
    camera->setView(viewDirection, upDirection);
}

void CameraController::attachCamera(Camera *cam) {
    camera = cam;

    position = camera->getPosition();
    viewDirection = camera->getViewDirection();
    upDirection = camera->getUpDirection();
    rightDirection = glm::normalize(glm::cross(viewDirection, upDirection));
}