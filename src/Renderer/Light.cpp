#include "Light.h"

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
