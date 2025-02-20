#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Math {
    inline glm::mat4 rescaleMatrix(const glm::mat4 &mat, const float scale = 1.0f) {
        glm::vec3 xAxis = glm::normalize(glm::vec3(mat[0]));
        glm::vec3 yAxis = glm::normalize(glm::vec3(mat[1]));
        glm::vec3 zAxis = glm::normalize(glm::vec3(mat[2]));

        glm::mat4 newMat = mat;
        newMat[0] = glm::vec4(xAxis, 0.0f) * scale;
        newMat[1] = glm::vec4(yAxis, 0.0f) * scale;
        newMat[2] = glm::vec4(zAxis, 0.0f) * scale;

        return newMat;
    }
}
