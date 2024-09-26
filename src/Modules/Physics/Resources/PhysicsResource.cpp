#include "PhysicsResource.h"
#include "../Helpers/PhysicsTypeCast.h"
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

JPH_SUPPRESS_WARNINGS
using namespace JPH;
using namespace JPH::literals;

static void TraceImpl(const char *inFMT, ...) {
    // Format the message
    va_list list;
    va_start(list, inFMT);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFMT, list);
    va_end(list);

    Log::write(buffer);
}

static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, uint inLine) {
    Log::error(inFile, ":", inLine, ": (", inExpression, ") ", (inMessage != nullptr ? inMessage : ""));

    return true;
};

PhysicsResource::PhysicsResource() : Resource(),
                                     m_PhysicsSystem(),
                                     m_objectPairFilter(),
                                     m_broadPhaseFilter(),
                                     m_broadPhaseLayerMapper(),
                                     m_contactListener(),
                                     m_jobPool(nullptr),
                                     m_tempAllocator(nullptr) {
}

Resource::Status PhysicsResource::fetchData(ResourceManager &) {
    return Resource::STATUS_DATA_READY;
}

/**
 * Note: Physics simulation configuration and details can be loaded from the json config file. So that each scene could
 * have its own physics settings applied.
 */
Resource::Status PhysicsResource::build() {
    RegisterDefaultAllocator();
    Trace = TraceImpl;
    AssertFailed = AssertFailedImpl;
    JPH::Factory::sInstance = new JPH::Factory();
    RegisterTypes();

    m_jobPool = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, (int) thread::hardware_concurrency() - 1);
    m_tempAllocator = new TempAllocatorImpl(10 * 1024 * 1024);

    const uint cMaxBodies = 1024;
    const uint cNumBodyMutexes = 0;
    const uint cMaxBodyPairs = 1024;
    const uint cMaxContactConstraints = 1024;

    m_PhysicsSystem.Init(cMaxBodies,
                         cNumBodyMutexes,
                         cMaxBodyPairs,
                         cMaxContactConstraints,
                         m_broadPhaseLayerMapper,
                         m_broadPhaseFilter,
                         m_objectPairFilter);

    m_PhysicsSystem.SetGravity({0, -9.8, 0});
    m_PhysicsSystem.SetContactListener(&m_contactListener);

    return Resource::STATUS_READY;
}

void PhysicsResource::destroy() {
    UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

float PhysicsResource::characterRayCast(glm::vec3 p, glm::vec3 d, Identity::Type characterEntityId) {

    return 0;
}

//bool
//PhysicsResource::characterRayCast(glm::vec3 p, glm::vec3 d, Identity::Type characterEntityId, RayCastResult &result) {
////    btVector3 rayStart = PhysicsTypeCast::glmToBullet(p);
////    btVector3 rayEnd = PhysicsTypeCast::glmToBullet(p + (d * 100.0f));
////
//////    PhysicsRayCastFilterCallback rayCallback(rayStart, rayEnd);
//////    rayCallback.m_excludedEntityId = characterEntityId;
////    btCollisionWorld::ClosestRayResultCallback rayCallback(rayStart, rayEnd);
////
////    m_dynamicsWorld->rayTest(rayStart, rayEnd, rayCallback);
////    if (rayCallback.hasHit()) {
////        const btCollisionObject* hitObject = rayCallback.m_collisionObject;
////        auto *userData = (PhysicsUserData *) hitObject->getUserPointer();
////        result.m_entityId = userData->m_entityId;
////        result.m_touchPoint = PhysicsTypeCast::bulletToGlm(rayCallback.m_hitPointWorld);
////        result.m_distance = rayCallback.m_closestHitFraction;
////
////        return true;
////    }
//
//    return false;
//}

void PhysicsResource::addContactPoint(Identity::Type entityId, glm::vec3 point) {
    if (m_entityContacts.count(entityId) == 0) {
        m_entityContacts[entityId].reserve(16);
    }

    m_entityContacts[entityId].push_back(point);
}

void PhysicsResource::clearEntityContacts() {
    for (auto &pair: m_entityContacts) {
        pair.second.clear();
    }
}

void PhysicsResource::simulate() {
    m_PhysicsSystem.Update(1.f / 60.f, 1, m_tempAllocator, m_jobPool);
}

JPH::BodyInterface &PhysicsResource::getInterface() {
    return m_PhysicsSystem.GetBodyInterface();
}

JPH::PhysicsSystem &PhysicsResource::getSystem() {
    return m_PhysicsSystem;
}
