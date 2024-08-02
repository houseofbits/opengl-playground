#include "PhysicsSystem.h"
#include "../../../Helper/Time.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

using namespace physx;

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
PxTolerancesScale toleranceScale;

PhysicsSystem::PhysicsSystem() : EntitySystem(), m_pxFoundation(nullptr), m_pxPhysics(nullptr), m_pxScene(nullptr), m_isSimulationDisabled(false) {
    usesComponent<RigidBodyComponent>();
    usesComponent<TransformComponent>();
}

void PhysicsSystem::initialize(ResourceManager *resourceManager) {
    m_pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (!m_pxFoundation) {
        std::cout << "Failed to create foundation" << std::endl;
        return;
    }

    toleranceScale.length = 1;
    toleranceScale.speed = 981;

    m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, toleranceScale);
    if (!m_pxPhysics) {
        std::cout << "Failed to create PhysX device" << std::endl;

        return;
    }

    PxSceneDesc sceneDesc(m_pxPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

    PxDefaultCpuDispatcher *m_cpuDispatcher = PxDefaultCpuDispatcherCreate(2);

    if (!m_cpuDispatcher) std::cerr << "PxDefaultCpuDispatcherCreate failed!" << std::endl;

    sceneDesc.cpuDispatcher = m_cpuDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    m_pxScene = m_pxPhysics->createScene(sceneDesc);
    if (!m_pxScene) {
        std::cerr << "createScene failed!" << std::endl;

        return;
    }
    m_pxScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0);
    m_pxScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

    //Ground plane
    PxMaterial *m_material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);
    physx::PxRigidStatic *groundPlane = PxCreatePlane(*m_pxPhysics, physx::PxPlane(0, 1, 0, 0), *m_material);
    m_pxScene->addActor(*groundPlane);
}

void PhysicsSystem::process() {

    buildRigidBodies();

    if (!m_isSimulationDisabled) {
        //    std::cout<<Time::frameTime<<" "<<1.0 / 60.0<<std::endl;
        m_pxScene->simulate(1.0 / 60.0);
        while (!m_pxScene->fetchResults(true)) {
        }
    }

    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody != nullptr) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);
            if (m_isSimulationDisabled) {
                rigidBody.second->m_pxRigidBody->setGlobalPose(transform->getPxTransform());
                rigidBody.second->m_pxInitialTransform = rigidBody.second->m_pxRigidBody->getGlobalPose();
            } else {
                transform->setFromPxTransform(rigidBody.second->m_pxRigidBody->getGlobalPose());
            }
        }
    }
}

void PhysicsSystem::buildRigidBodies() {
    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody == nullptr) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);

            PxMaterial *m_material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);
            float halfExtent = .5f;
            physx::PxShape *shape = m_pxPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_material);
            rigidBody.second->m_pxRigidBody = m_pxPhysics->createRigidDynamic(transform->getPxTransform());
            rigidBody.second->m_pxInitialTransform = transform->getPxTransform();
            rigidBody.second->m_pxRigidBody->attachShape(*shape);
            physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody.second->m_pxRigidBody, 100.0f);
            m_pxScene->addActor(*rigidBody.second->m_pxRigidBody);
            shape->release();
        }
    }
}

void PhysicsSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &PhysicsSystem::handleEditorUIEvent);
}

bool PhysicsSystem::handleEditorUIEvent(EditorUIEvent *event) {

    if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if(event->m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }

    return true;
}

void PhysicsSystem::resetToInitialTransform() {
    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody != nullptr) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);

            transform->setFromPxTransform(rigidBody.second->m_pxInitialTransform);
            rigidBody.second->m_pxRigidBody->setGlobalPose(rigidBody.second->m_pxInitialTransform);
        }
    }
}
