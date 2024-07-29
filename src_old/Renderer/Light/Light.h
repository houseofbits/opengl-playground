#pragma once

#include "../Camera/Camera.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef struct alignas(16)
{
    // 0
    glm::vec3 position; // 12
    float intensity;    // 40

    // 16
    glm::vec3 color;              // 12
    unsigned int doesCastShadows; // 4

    // 32
    glm::vec3 direction;        // 12
    unsigned int isPointSource; // 4

    // 48
    float distAttenMin;            // 4
    float distAttenMax;            // 4
    float beamAngle;               // 4
    unsigned int shadowAtlasIndex; // 4

    // 64
    glm::mat4 projectionViewMatrix; // 64

    // 128
    unsigned int projectionTextureId;   //4

} LightUniform;

// Defines light projection view matrices and index into shadow atlas
// for shadow calculation
// Spot light has one view. Point light will have up to 6 views
// Additionally, there is a possibility to split the light frustum into multiple views
// for lights covering large area, to get better shadow quality.
class LightView
{
public:
    glm::mat4 viewMatrix = glm::mat4(1.0);
    glm::mat4 projectionMatrix = glm::mat4(1.0);
    glm::mat4 projectionViewMatrix = glm::mat4(1.0);
    unsigned int shadowAtlasIndex = 0;

    // bool shouldUpdate = {true};      //TODO
    // bool shouldRelocateAtlas = {true};

    void calculateProjectionViewMatrix(float fov, glm::vec3 position, glm::vec3 direction, float farPlane)
    {
        glm::vec3 dir = glm::normalize(direction);
        glm::vec3 up(0,1,0);
        if (fabs(glm::dot(dir, up)) > 0.99) {
            up = glm::vec3(1,0,0);
        }

        glm::vec3 cross = glm::cross(dir, up);
        glm::vec3 tangentUp = glm::cross(dir, cross);

        projectionMatrix = glm::perspective<float>(glm::radians(fov), 1.0, 0.01, farPlane);
        viewMatrix = glm::lookAt(position, position + direction, tangentUp);
        projectionViewMatrix = projectionMatrix * viewMatrix;
    }
};

class Light
{
public:
    Light();

    enum Type
    {
        POINT = 0,
        SPOT = 1,
        DIRECT = 2,
    };

    Type type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float distAttenMin;
    float distAttenMax;
    float beamAngle;
    float falloffAngle;
    bool doesCastShadows;

    unsigned int projectionTextureId;

    // Obsolete stuff
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 projectionViewMatrix;
    unsigned int uniformBufferIndex;
    glm::mat4 &getProjectionViewMatrix();

    unsigned int calculateMaximumTextureSize();

    // New stuff
    LightView views[6];
    unsigned int numberOfViews;

    void generateViews();
};