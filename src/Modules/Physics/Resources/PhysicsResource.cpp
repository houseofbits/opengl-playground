#include "PhysicsResource.h"

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;
physx::PxTolerancesScale toleranceScale;

PhysicsResource::PhysicsResource() : Resource(),
                                     m_pxFoundation(nullptr),
                                     m_pxPhysics(nullptr),
                                     m_pxScene(nullptr),
                                     m_ControllerManager(nullptr) {
}

Resource::Status PhysicsResource::fetchData(ResourceManager &) {
    return Resource::STATUS_DATA_READY;
}

Resource::Status PhysicsResource::build() {

    m_pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (!m_pxFoundation) {
        Log::error("PhysicsResource: Failed to create foundation");

        return Resource::STATUS_BUILD_ERROR;
    }

    toleranceScale.length = 1;
    toleranceScale.speed = 981;

    m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, toleranceScale);
    if (!m_pxPhysics) {
        Log::error("PhysicsResource: Failed to create PhysX device");

        return Resource::STATUS_BUILD_ERROR;
    }

    physx::PxSceneDesc sceneDesc(m_pxPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

    physx::PxDefaultCpuDispatcher *m_cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);

    if (!m_cpuDispatcher) {
        Log::error("PhysicsResource: PxDefaultCpuDispatcherCreate failed!");

        return Resource::STATUS_BUILD_ERROR;
    }

    sceneDesc.cpuDispatcher = m_cpuDispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

    m_pxScene = m_pxPhysics->createScene(sceneDesc);
    if (!m_pxScene) {
        Log::error("PhysicsResource: createScene failed!");

        return Resource::STATUS_BUILD_ERROR;
    }
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0);
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

    m_ControllerManager = PxCreateControllerManager(*m_pxScene, true);

    return Resource::STATUS_READY;
}

void PhysicsResource::destroy() {
}
