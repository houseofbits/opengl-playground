#include <cstdarg>
#include "PhysicsResource.h"
#include "../Helpers/PhysicsTypeCast.h"
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>

JPH_SUPPRESS_WARNINGS
using namespace JPH;
using namespace JPH::literals;

static void TraceImpl(const char *inFMT, ...) {
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
                                     m_broadPhaseFilter(),
                                     m_broadPhaseLayerMapper(),
                                     m_contactListener(),
                                     m_jobPool(nullptr),
                                     m_tempAllocator(nullptr),
                                     m_debugRenderer(nullptr) {
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

    m_jobPool = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers,
                                        static_cast<int>(thread::hardware_concurrency()) - 1);
    m_tempAllocator = new TempAllocatorImpl(10 * 1024 * 1024);

    constexpr uint cMaxBodies = 1024;
    constexpr uint cNumBodyMutexes = 0;
    constexpr uint cMaxBodyPairs = 1024;
    constexpr uint cMaxContactConstraints = 1024;

    m_PhysicsSystem.Init(cMaxBodies,
                         cNumBodyMutexes,
                         cMaxBodyPairs,
                         cMaxContactConstraints,
                         m_broadPhaseLayerMapper,
                         m_broadPhaseFilter,
                         m_objectPairFilter);

    m_PhysicsSystem.SetGravity({0, -9.8, 0});
    m_PhysicsSystem.SetContactListener(&m_contactListener);
    m_contactListener.setPhysicsResource(*this);

    m_debugRenderer = PhysicsDebugRenderer::getInstance();

    return Resource::STATUS_READY;
}

void PhysicsResource::destroy() {
    UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}

void PhysicsResource::destroyAllBodies() {
    auto constraints = m_PhysicsSystem.GetConstraints();
    for (const auto &constraint: constraints) {
        m_PhysicsSystem.RemoveConstraint(constraint);
    }

    BodyIDVector bodyIDs;
    m_PhysicsSystem.GetBodies(bodyIDs);
    for (JPH::BodyID bodyID: bodyIDs) {
        m_PhysicsSystem.GetBodyInterface().RemoveBody(bodyID);
        m_PhysicsSystem.GetBodyInterface().DestroyBody(bodyID);
    }
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

const JPH::BodyLockInterfaceLocking &PhysicsResource::getLockInterface() const {
    return m_PhysicsSystem.GetBodyLockInterface();
}

JPH::PhysicsSystem &PhysicsResource::getSystem() {
    return m_PhysicsSystem;
}

void PhysicsResource::drawDebug(ShaderProgramResource &shader) {
    m_debugRenderer->NextFrame();

    JPH::BodyManager::DrawSettings drawSettings;

    drawSettings.mDrawShapeColor = BodyManager::EShapeColor::MaterialColor;

    m_debugRenderer->m_shader = &shader;

    m_PhysicsSystem.DrawBodies(drawSettings, m_debugRenderer);
}
