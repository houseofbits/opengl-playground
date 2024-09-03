#pragma once
#include "PhysicsActorUserData.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

class ExcludeEntityIdFilterCallback : public physx::PxQueryFilterCallback {
public:
    Identity::Type excludedEntityId;

    physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) override {
        auto* data = static_cast<PhysicsActorUserData*>(actor->userData);
        if (excludedEntityId == data->entityId) {
            return physx::PxQueryHitType::eNONE;
        }

        return physx::PxQueryHitType::eBLOCK;
    }

    physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor) override {
        return physx::PxQueryHitType::eBLOCK;
    }
};