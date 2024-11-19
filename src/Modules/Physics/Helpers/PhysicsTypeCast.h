#pragma once

#include "../../Common/Components/TransformComponent.h"
#include <glm/mat4x4.hpp>

namespace PhysicsTypeCast {

    inline JPH::Vec3 glmToJPH(glm::vec3 p) {
        return {p.x, p.y, p.z};
    }

    inline JPH::Quat glmToJPH(glm::quat q) {
        return {q.x, q.y, q.z, q.w};
    }

    inline glm::vec3 JPHToGlm(JPH::Vec3 p) {
        return {p.GetX(), p.GetY(), p.GetZ()};
    }

    inline glm::quat JPHToGlm(JPH::Quat q) {
        return {q.GetW(), q.GetX(), q.GetY(), q.GetZ()};
    }

    inline void applyJPHMat44ToTransformComponent(TransformComponent &transformComponent, JPH::Mat44 &m4) {
        glm::vec3 scale = transformComponent.getScale();
        transformComponent.resetTransform();
        transformComponent.setTranslation(JPHToGlm(m4.GetTranslation()));
        transformComponent.setRotation(JPHToGlm(m4.GetQuaternion()));
        transformComponent.setScale(scale);
    }

    inline glm::mat4 JPHToGlm(JPH::Mat44 m) {
        glm::mat4 glmMat;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                glmMat[col][row] = m(row, col);
            }
        }
        
        return glmMat;
    }
};