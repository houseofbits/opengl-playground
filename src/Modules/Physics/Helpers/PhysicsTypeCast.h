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

//
//    inline btMatrix3x3 glmToBullet(glm::mat4 m1) {
//        glm::mat4 m = glm::mat4(1.0);
//        return {
//                PhysicsTypeCast::glmToBullet(m[0]),
//                PhysicsTypeCast::glmToBullet(m[1]),
//                PhysicsTypeCast::glmToBullet(m[2])};
//    }
//
//    inline btTransform createBtTransformFromTransformComponent(TransformComponent &transformComponent) {
//        btTransform t;
//        t.setIdentity();
//        t.setRotation(glmToBullet(transformComponent.getRotation()));
//        t.setOrigin(glmToBullet(transformComponent.getTranslation()));
//
//        return t;
//    }
//
//    inline btTransform createBtTransformFromTranslation(glm::vec3 translation) {
//        btTransform t;
//        t.setIdentity();
//        t.setOrigin(glmToBullet(translation));
//
//        return t;
//    }
//
//    inline void applyBtTransformToTransformComponent(TransformComponent &transformComponent, btTransform &t) {
//        glm::vec3 scale = transformComponent.getScale();
//        transformComponent.resetTransform();
//        transformComponent.setTranslation(bulletToGlm(t.getOrigin()));
//        transformComponent.setRotation(bulletToGlm(t.getRotation()));
//        transformComponent.setScale(scale);
//    }
};// namespace PhysicsTypeCast