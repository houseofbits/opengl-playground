#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../Events/EventManager.h"

class WindowEvent;
class InputEvent;

class Camera
{
private:
    float verticalAngle;
    float horizontalAngle;
    glm::vec3 right;
    glm::vec3 up;
    bool isTransformDirty;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionViewMatrix;
    glm::vec3 direction;
    glm::vec3 position;

    void calculateView();
    void calculatePV();

public:
    Camera();

    void init(unsigned int viewportWidth, unsigned int viewportHeight);
    void resize(unsigned int viewportWidth, unsigned int viewportHeight);
    void registerEventHandlers(EventManager *eventManager);
    bool handleWindowEvent(WindowEvent *const event);
    bool handleInputEvent(InputEvent *const event);
    void setViewMatrix(glm::mat4 m)
    {
        viewMatrix = m;
        isTransformDirty = true;
    }
    void setProjectionMatrix(glm::mat4 m)
    {
        projectionMatrix = m;
        isTransformDirty = true;
    }
    glm::mat4 &getProjectionViewMatrix();
    glm::vec3 &getPosition() { return position; };
};
