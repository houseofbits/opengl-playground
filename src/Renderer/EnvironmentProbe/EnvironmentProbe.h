#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef struct alignas(16) {
    // 0
    glm::vec3 position;              // 12
    unsigned int cubeMapTextureLayer;// 4
    //16
    glm::vec4 boundingBoxMin;// 16
    //32
    glm::vec4 boundingBoxMax;// 16
    //48
} EnvironmentProbeUniform;

class EnvironmentProbe {
public:
    EnvironmentProbe();

    unsigned int cubeMapTextureLayer;
    glm::vec3 position;
    glm::vec3 boundingBoxMin;       //Bounding volume extents in world space
    glm::vec3 boundingBoxMax;
    int priority;
    float padding;
};
