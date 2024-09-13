#pragma once
#include "PhysicsActorUserData.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

class PhysicsResource;

class SimulationEventCallback : public physx::PxSimulationEventCallback {
public:
    explicit SimulationEventCallback(PhysicsResource *physics) : physx::PxSimulationEventCallback(), m_physics(physics) {}

    void onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs) override;
    void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count) override {}
    void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count) override {}
    void onWake(physx::PxActor **actors, physx::PxU32 count) override {}
    void onSleep(physx::PxActor **actors, physx::PxU32 count) override {}
    void onAdvance(const physx::PxRigidBody *const *, const physx::PxTransform *, const physx::PxU32) override {}

    PhysicsResource *m_physics;
};
