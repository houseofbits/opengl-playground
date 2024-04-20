#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

enum LightUniformFlags
{
    POINT_SOURCE = 1 << 0,
    DIRECT_SOURCE = 1 << 1,
    HAS_SHADOW = 1 << 2,
};

typedef struct alignas(16)
{
    glm::vec3 position;
    float falloff;

    glm::vec3 color;
    float intensity;

    glm::vec3 direction;
    unsigned int flags;

    float distAttenMin;
    float distAttenMax;
    float beamAngle;
    float falloffAngle;

    glm::vec2 shadowAtlasPos;
    glm::vec2 shadowAtlasSize;
    glm::mat4 projectionViewMatrix;

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
    unsigned int uniformBufferIndex = 0;

    // bool shouldUpdate = {true};      //TODO
    // bool shouldRelocateAtlas = {true};

    void calculateProjectionViewMatrix(float fov, glm::vec3 position, glm::vec3 direction, float farPlane)
    {
        projectionMatrix = glm::perspective<float>(glm::radians(fov), 1.0, 0.01, farPlane);
        viewMatrix = glm::lookAt(position, position + direction, glm::vec3(1, 1, 1));
        projectionViewMatrix = projectionMatrix * viewMatrix;
    }
};

class Light
{
public:
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

    // Multilight stuff
    // glm::mat4 viewMatrices[MAX_VIEWS]; // Multiple view matrices in case of point light
    // glm::vec4 shadowAtlasPosition[MAX_VIEWS];

    // Obsolette stuff
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 projectionViewMatrix;
    unsigned int uniformBufferIndex;
    glm::mat4 &getProjectionViewMatrix();

    // New stuff
    LightView views[6];
    unsigned int numberOfViews;

    void generateViews();
    unsigned int getLightUniformFlags(bool withShadows);
};