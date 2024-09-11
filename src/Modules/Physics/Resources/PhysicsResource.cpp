#include "PhysicsResource.h"
#include "../Helpers/SceneFilterShader.h"

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;
physx::PxTolerancesScale toleranceScale;

PhysicsResource::PhysicsResource() : Resource(),
                                     m_pxFoundation(nullptr),
                                     m_pxPhysics(nullptr),
                                     m_pxScene(nullptr),
                                     m_SimulationEventCallback(this) {
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
    sceneDesc.solverType = physx::PxSolverType::Enum::eTGS;
    sceneDesc.simulationEventCallback = &m_SimulationEventCallback;
    sceneDesc.filterShader = &SceneFilterShader;

    physx::PxDefaultCpuDispatcher *m_cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    if (!m_cpuDispatcher) {
        Log::error("PhysicsResource: PxDefaultCpuDispatcherCreate failed!");

        return Resource::STATUS_BUILD_ERROR;
    }

    sceneDesc.cpuDispatcher = m_cpuDispatcher;

    m_pxScene = m_pxPhysics->createScene(sceneDesc);
    if (!m_pxScene) {
        Log::error("PhysicsResource: createScene failed!");

        return Resource::STATUS_BUILD_ERROR;
    }
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0);
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

    return Resource::STATUS_READY;
}

void PhysicsResource::destroy() {
    m_pxScene->release();
    //    m_ControllerManager->release();
    //    m_pxPhysics->release();
    //    m_pxFoundation->release();
}

float PhysicsResource::rayCast(glm::vec3 p, glm::vec3 d, float maxDistance, physx::PxQueryFilterCallback *callback) const {
    physx::PxVec3 origin(p.x, p.y, p.z);
    physx::PxVec3 unitDir(d.x, d.y, d.z);
    physx::PxRaycastBuffer hit;

    physx::PxQueryFilterData filterData;
    filterData.flags |= physx::PxQueryFlag::ePREFILTER;
    //    filterData.flags |= physx::PxQueryFlag::ePOSTFILTER; // If you're using postFilter

    if (m_pxScene->raycast(origin, unitDir, maxDistance, hit, physx::PxHitFlags(physx::PxHitFlag::eDEFAULT), filterData, callback)) {
        //        std::cout << hit.getNbAnyHits() << std::endl;
        std::cout << "hit " << hit.block.distance << " = " << hit.block.position.x << "," << hit.block.position.y << "," << hit.block.position.z << std::endl;
        return hit.block.distance;
    }

    return 0.0;
}

float PhysicsResource::characterRayCast(glm::vec3 p, glm::vec3 d, Identity::Type characterEntityId) {
    physx::PxVec3 origin(p.x, p.y, p.z);
    physx::PxVec3 unitDir(d.x, d.y, d.z);
    physx::PxRaycastBuffer hit;

    physx::PxQueryFilterData filterData;
    filterData.flags |= physx::PxQueryFlag::ePREFILTER;

    m_excludeEntityIdFilter.excludedEntityId = characterEntityId;

    if (m_pxScene->raycast(origin, unitDir, 100.0, hit, physx::PxHitFlags(physx::PxHitFlag::eDEFAULT), filterData, (physx::PxQueryFilterCallback *) &m_excludeEntityIdFilter)) {
        //        std::cout << "character " << hit.block.distance << " = " << hit.block.position.x << "," << hit.block.position.y << "," << hit.block.position.z << std::endl;
        return hit.block.distance;
    }

    return 0;
}

bool PhysicsResource::characterRayCast(glm::vec3 p, glm::vec3 d, Identity::Type characterEntityId, RayCastResult &result) {
    physx::PxVec3 origin(p.x, p.y, p.z);
    physx::PxVec3 unitDir(d.x, d.y, d.z);
    physx::PxRaycastBuffer hit;

    physx::PxQueryFilterData filterData;
    filterData.flags |= physx::PxQueryFlag::ePREFILTER;

    m_excludeEntityIdFilter.excludedEntityId = characterEntityId;
    if (m_pxScene->raycast(origin, unitDir, 100.0, hit, physx::PxHitFlags(physx::PxHitFlag::eDEFAULT), filterData, (physx::PxQueryFilterCallback *) &m_excludeEntityIdFilter)) {
        auto *data = static_cast<PhysicsActorUserData *>(hit.block.actor->userData);

        result.m_entityId = data->entityId;
        result.m_distance = hit.block.distance;
        result.m_touchPoint = {hit.block.position.x, hit.block.position.y, hit.block.position.z};

        return true;
    }

    return false;
}

void PhysicsResource::addContactPoint(Identity::Type entityId, const physx::PxContactPairPoint& contact) {
    if (m_entityContacts.count(entityId) == 0) {
        m_entityContacts[entityId].reserve(16);
    }

    m_entityContacts[entityId].push_back(contact);
}

void PhysicsResource::clearEntityContacts() {
    for(auto &pair : m_entityContacts) {
       pair.second.clear();
    }
}
