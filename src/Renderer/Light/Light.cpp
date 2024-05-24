#include "Light.h"
#include "../Shader/UniformBuffer.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Light::Light(): projectionTextureId(0)
{
    type = POINT;
    position = glm::vec3(0.0);
    direction = glm::vec3(0.0);
    color = glm::vec3(0.0);
    distAttenMin = 0;
    distAttenMax = 0;
    intensity = 1;
    doesCastShadows = false;
    projectionTextureId = 0;
    beamAngle = 45;
}

glm::mat4 &Light::getProjectionViewMatrix()
{
    projectionMatrix = glm::perspective<float>(glm::radians(beamAngle), 1.0, 0.01, distAttenMax);
    viewMatrix = glm::lookAt(position, position + direction, glm::vec3(1, 0, 0));

    projectionViewMatrix = projectionMatrix * viewMatrix;

    return projectionViewMatrix;
}

void Light::generateViews()
{
    if (type == SPOT)
    {
        numberOfViews = 1;
        views[0].calculateProjectionViewMatrix(beamAngle, position, direction, distAttenMax);
    }

    if (type == POINT)
    {
        glm::vec3 directions[] = {
            glm::vec3(1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, -1, 0),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, -1),
        };

        numberOfViews = 6;
        for (unsigned int i = 0; i < 6; i++)
        {
            views[i].calculateProjectionViewMatrix(90.0, position, directions[i], distAttenMax);
        }
    }
}

unsigned int Light::calculateMaximumTextureSize()
{
    float angle = beamAngle;
    if (type == POINT) {
        angle = 90.0;
    }

    int projectionSize = (int)(distAttenMax * (float)sin(glm::radians(angle / 2))) * 2;

    unsigned int lowerPower = 1u << static_cast<unsigned int>(std::log2(projectionSize));
    unsigned int upperPower = lowerPower << 1;

    unsigned int size;
//    if (projectionSize - lowerPower < upperPower - projectionSize) {
//        size = lowerPower;
//    } else {
        size = upperPower;
//    }

    size = size * 2;

    size = std::min(std::max((int)size, 256), 1024);

//    std::cout<<" b:"<<beamAngle<<" l:"<<distAttenMax<<std::endl;
//    std::cout<<" proj:"<<projectionSize<<" pow2:"<<size<<std::endl;
    return size;
}