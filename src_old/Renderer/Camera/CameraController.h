#pragma once

#include "Camera.h"

class CameraController {
private:
    void calculateTBN();
    void updateCamera();

    Camera* camera;
    glm::vec3 position;
    glm::vec3 viewDirection;
    glm::vec3 rightDirection;
    glm::vec3 upDirection;

public:
    CameraController();

    void registerEventHandlers(EventManager *eventManager);
    bool handleWindowEvent(WindowEvent *event);
    bool handleInputEvent(InputEvent *event);
    void attachCamera(Camera* camera);
};
