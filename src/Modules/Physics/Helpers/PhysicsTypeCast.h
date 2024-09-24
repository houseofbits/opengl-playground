#pragma once

#include "../../Common/Components/TransformComponent.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/mat4x4.hpp>

namespace PhysicsTypeCast {

    inline btVector3 glmToBullet(glm::vec3 p) {
        return {p.x, p.y, p.z};
    }

    inline glm::vec3 bulletToGlm(btVector3 p) {
        return {p.x(), p.y(), p.z()};
    }

    inline glm::quat bulletToGlm(btQuaternion q) {
        return {q.w(), q.x(), q.y(), q.z()};
    }

    inline btQuaternion glmToBullet(glm::quat q) {
        return {q.x, q.y, q.z, q.w};
    }

    inline btMatrix3x3 glmToBullet(glm::mat4 m1) {
        glm::mat4 m = glm::mat4(1.0);
        return {
                PhysicsTypeCast::glmToBullet(m[0]),
                PhysicsTypeCast::glmToBullet(m[1]),
                PhysicsTypeCast::glmToBullet(m[2])};
    }

    inline btTransform createBtTransformFromTransformComponent(TransformComponent &transformComponent) {
        btTransform t;
        t.setIdentity();
        t.setRotation(glmToBullet(transformComponent.getRotation()));
        t.setOrigin(glmToBullet(transformComponent.getTranslation()));

        return t;
    }

    inline btTransform createBtTransformFromTranslation(glm::vec3 translation) {
        btTransform t;
        t.setIdentity();
        t.setOrigin(glmToBullet(translation));

        return t;
    }

    inline void applyBtTransformToTransformComponent(TransformComponent &transformComponent, btTransform &t) {
        glm::vec3 scale = transformComponent.getScale();
        transformComponent.resetTransform();
        transformComponent.setTranslation(bulletToGlm(t.getOrigin()));
        transformComponent.setRotation(bulletToGlm(t.getRotation()));
        transformComponent.setScale(scale);
    }
};// namespace PhysicsTypeCast