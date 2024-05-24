#pragma once

#include "Camera.h"

class CameraController {
private:
    void calculateTBN();
    void updateCamera();

    Camera* camera;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;

    float verticalAngle;
    float horizontalAngle;

public:
    CameraController();

    void registerEventHandlers(EventManager *eventManager);
    bool handleWindowEvent(WindowEvent *event);
    bool handleInputEvent(InputEvent *event);
    void attachCamera(Camera* camera);
};
