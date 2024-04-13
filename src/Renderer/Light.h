#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

class Light
{
public:
    typedef struct alignas(16)
    {
        glm::vec3 position;
        float falloff;

        glm::vec3 color;
        float intensity;

        glm::vec3 direction;
        unsigned int type;

        float distAttenMin;
        float distAttenMax;
        float beamAngle;
        float falloffAngle;
    } ShaderLight;

    enum Type
    {
        POINT = 0,
        SPOT = 1,
        // DIRECT = 2
    };

    Type type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float distAttenMin;
    float distAttenMax;
    float beamAngle;    // Half of the angle in radians
    float falloffAngle; // Half of the angle in radians
    bool doesCastShadows;
    Camera camera;

    Camera &getCamera()
    {
        glm::vec3 up = glm::cross(direction, glm::vec3(1, 0, 0));

        camera.setProjectionMatrix(glm::perspective<float>(glm::radians(60.0), 1.0, 0.01, distAttenMax));
        camera.setViewMatrix(glm::lookAt(position, position + direction, glm::vec3(1, 0, 0)));

        return camera;
    }
};