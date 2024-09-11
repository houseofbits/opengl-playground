#pragma once
#include "FilterGroups.h"
#include "PhysicsActorUserData.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

physx::PxFilterFlags SceneFilterShader(
        physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
        physx::PxPairFlags &pairFlags, const void *constantBlock, physx::PxU32 constantBlockSize) {

    if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1)) {
        pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
        return physx::PxFilterFlag::eDEFAULT;
    }

    pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

    if (filterData0.word0 == FilterGroups::CHARACTER_CONTROLLER_CONTRACT_REPORTING || filterData1.word0 == FilterGroups::CHARACTER_CONTROLLER_CONTRACT_REPORTING) {
        pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
        pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
        pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
        pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
        pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
    }

    return physx::PxFilterFlag::eDEFAULT;
}