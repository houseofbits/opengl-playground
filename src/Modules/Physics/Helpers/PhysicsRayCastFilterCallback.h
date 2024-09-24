#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "../../../Core/Reflection/Identity.h"
#include "PhysicsUserData.h"

class PhysicsRayCastFilterCallback : public btCollisionWorld::ClosestRayResultCallback {
public:
    PhysicsRayCastFilterCallback(const btVector3 &rayFromWorld, const btVector3 &rayToWorld)
            : btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld), m_excludedEntityId(0) {}

    btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) override {
        auto *userDataA = (PhysicsUserData *) rayResult.m_collisionObject->getUserPointer();
        if (userDataA->m_entityId == m_excludedEntityId) {
            return 0.0f;
        }

        return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }

    Identity::Type m_excludedEntityId;
};
