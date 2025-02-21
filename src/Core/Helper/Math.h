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

    inline glm::quat getRotation(glm::mat4 m) {
        glm::vec3 scale;
        for (int i = 0; i < 3; i++) {
            scale[i] = glm::length(glm::vec3(m[i]));
        }

        const glm::mat3 rotMtx(
            glm::vec3(m[0]) / scale[0],
            glm::vec3(m[1]) / scale[1],
            glm::vec3(m[2]) / scale[2]);

        return glm::quat_cast(rotMtx);
    }

    inline glm::vec3 getTranslation(glm::mat4 m) {
        return m[3];
    }

    inline glm::vec3 getXAxis(const glm::mat4 &m) {
        return m[0];
    }

    inline glm::vec3 getYAxis(const glm::mat4 &m) {
        return m[1];
    }

    inline glm::vec3 getZAxis(const glm::mat4 &m) {
        return m[2];
    }

    inline glm::mat4 createMatrixFromTranslationAndRotation(glm::vec3 translation, glm::quat rotation) {
        glm::mat4 m(1.0);
        m = glm::translate(m, translation);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        m *= rotationMatrix;

        return m;
    }
}
