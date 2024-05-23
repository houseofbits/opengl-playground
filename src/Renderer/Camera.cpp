#include "Camera.h"
#include "../Helper/Time.h"
#include "../Events/WindowEvent.h"
#include "../Events/InputEvent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <iostream>

Camera::Camera() : verticalAngle(-0.4), horizontalAngle(-0.6), position(0), isTransformDirty(true), projectionMatrix(0.0), viewMatrix(0.0), projectionViewMatrix(0.0)
{
    position = glm::vec3(145.788, 163.927, 237.684);
    verticalAngle = -0.219405;
    horizontalAngle = -2.77744;
}

void Camera::resize(unsigned int viewportWidth, unsigned int viewportHeight)
{
    calculateView();
    projectionMatrix = glm::perspective(glm::radians(90.0f), (float)viewportWidth / viewportHeight, 0.1f, 1000.0f);
    isTransformDirty = true;
}

void Camera::registerEventHandlers(EventManager *eventManager)
{
    eventManager->registerEventReceiver(this, &Camera::handleWindowEvent);
    eventManager->registerEventReceiver(this, &Camera::handleInputEvent);
}

bool Camera::handleWindowEvent(WindowEvent *const event)
{
    if (event->eventType == WindowEvent::RESIZE || event->eventType == WindowEvent::CREATE)
    {
        resize(event->window->viewportWidth, event->window->viewportHeight);
    }

    return true;
}

bool Camera::handleInputEvent(InputEvent *const event)
{
    float moveSpeed = 4;
    float lookSpeed = 0.1;

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft)
    {
        horizontalAngle += lookSpeed * Time::frameTime * event->mouseMotion.x;
        verticalAngle += lookSpeed * Time::frameTime * event->mouseMotion.y;
        calculateView();
    }

    if (event->type == InputEvent::MOUSEMOVE && event->mouseButtonRight)
    {
        position += up * (moveSpeed * event->mouseMotion.y * Time::frameTime);
        position -= right * (moveSpeed * event->mouseMotion.x * Time::frameTime);
        calculateView();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 26)
    {
        position += direction * (moveSpeed * Time::frameTime);
        calculateView();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 22)
    {
        position -= direction * (moveSpeed * Time::frameTime);
        calculateView();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 4)
    {
        position -= right * (moveSpeed * Time::frameTime);
        calculateView();
    }

    if (event->type == InputEvent::KEYPRESS && event->keyCode == 7)
    {
        position += right * (moveSpeed * Time::frameTime);
        calculateView();
    }

    if (event->type == InputEvent::KEYDOWN && event->keyCode == 58) // F1
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

void Camera::calculateView()
{
    isTransformDirty = true;

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

    viewMatrix = glm::lookAt(position, position + direction, up);
}

glm::mat4 &Camera::getProjectionViewMatrix()
{
    if (isTransformDirty)
    {
        projectionViewMatrix = projectionMatrix * viewMatrix;
        isTransformDirty = false;
    }

    return projectionViewMatrix;
}

Camera &Camera::setPosition(glm::vec3 pos)
{
    position = pos;

    return *this;
}

Camera &Camera::setAngles(float horizontal, float vertical)
{
    verticalAngle = glm::radians(vertical);
    horizontalAngle = glm::radians(horizontal);

    return *this;
}

void Camera::bind(Shader &shader)
{
    shader.setUniform("viewProjectionMatrix", getProjectionViewMatrix());
    shader.setUniform("viewPosition", getPosition());
}
