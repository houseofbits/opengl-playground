#pragma once
#include "../Events/EventManager.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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

    void resize(unsigned int viewportWidth, unsigned int viewportHeight);
    void registerEventHandlers(EventManager *eventManager);
    bool handleWindowEvent(WindowEvent *event);
    bool handleInputEvent(InputEvent *event);
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
    glm::mat4 &getViewMatrix() { return viewMatrix; }
    glm::mat4 &getProjectionMatrix() { return projectionMatrix; }
    glm::vec3 &getPosition() { return position; };
    glm::vec3 &getDirection() { return direction; };

    Camera &setPosition(glm::vec3 position);
    Camera &setAngles(float horizontal, float vertical);

    void bind(Shader& shader);
};
