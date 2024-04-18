#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

class Light
{
public:
    static const unsigned int MAX_VIEWS = 6;

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
        // DIRECT = 2,
        // POINT_SHADOW  = 3,
        SPOT_SHADOW = 4,
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
    // Camera camera;

    // Multilight stuff
    // glm::mat4 viewMatrices[MAX_VIEWS]; // Multiple view matrices in case of point light
    // glm::vec4 shadowAtlasPosition[MAX_VIEWS];

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 projectionViewMatrix;
    glm::vec4 shadowAtlasPosition;

    unsigned int uniformBufferIndex;

    // Camera &getCamera()
    // {
    //     glm::vec3 up = glm::cross(direction, glm::vec3(1, 0, 0));

    //     camera.setProjectionMatrix(glm::perspective<float>(glm::radians(60.0), 1.0, 0.01, distAttenMax));
    //     camera.setViewMatrix(glm::lookAt(position, position + direction, glm::vec3(1, 0, 0)));

    //     return camera;
    // }

    glm::mat4 &getProjectionViewMatrix()
    {
        projectionMatrix = glm::perspective<float>(glm::radians(beamAngle), 1.0, 0.01, distAttenMax);
        viewMatrix = glm::lookAt(position, position + direction, glm::vec3(1, 0, 0));

        projectionViewMatrix = projectionMatrix * viewMatrix;

        return projectionViewMatrix;
    }

    void setShadowAtlasPosition(float x, float y, float width, float height)
    {
        shadowAtlasPosition.x = x;
        shadowAtlasPosition.y = y;
        shadowAtlasPosition.z = width;
        shadowAtlasPosition.w = height;
    }
};