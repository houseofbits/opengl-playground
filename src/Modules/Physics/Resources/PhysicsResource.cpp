#include "PhysicsResource.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "Jolt/Physics/Collision/CastResult.h"
#include "Jolt/Physics/Collision/RayCast.h"
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
                                     m_debugRenderer(nullptr),
                                     m_tempAllocator(nullptr) {
}

Resource::Status PhysicsResource::fetchData(ResourceManager &) {
    return Resource::STATUS_DATA_READY;
}

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

    m_debugRenderer = PhysicsDebugRenderer::getInstance();

    return Resource::STATUS_READY;
}

void PhysicsResource::destroy() {
    UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

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

void PhysicsResource::drawDebug(ShaderProgramResource &shader) {
    m_debugRenderer->NextFrame();
    
    JPH::BodyManager::DrawSettings drawSettings;

    m_debugRenderer->m_shader = &shader;

    m_PhysicsSystem.DrawBodies(drawSettings, m_debugRenderer);
}
