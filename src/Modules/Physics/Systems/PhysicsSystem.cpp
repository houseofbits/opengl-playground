#include "PhysicsSystem.h"
#include "../../../Core/Helper/Time.h"
#include "../../../Core/Helper/Types.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/CharacterControllerComponent.h"
#include "../Components/RigidBodyComponent.h"

using namespace physx;

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
static PxControllerFilters filters;
PxTolerancesScale toleranceScale;

PhysicsSystem::PhysicsSystem() : EntitySystem(),
                                 m_pxFoundation(nullptr),
                                 m_pxPhysics(nullptr),
                                 m_pxScene(nullptr),
                                 m_isSimulationDisabled(false),
                                 m_ControllerManager(nullptr) {
    usesComponent<RigidBodyComponent>();
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<CharacterControllerComponent>();
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

    m_ControllerManager = PxCreateControllerManager(*m_pxScene, true);

    //Test ground plane
    PxMaterial *m_material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);
    physx::PxRigidStatic *groundPlane = PxCreatePlane(*m_pxPhysics, physx::PxPlane(0, 1, 0, 0), *m_material);
    m_pxScene->addActor(*groundPlane);
}

void PhysicsSystem::process() {

    buildRigidBodies();
    buildCCTs();

    if (!m_isSimulationDisabled) {
        m_pxScene->simulate(1.0 / 60.0);
        while (!m_pxScene->fetchResults(true)) {
        }
    }

    updateRigidBodies();
    updateCCTs();
}

void PhysicsSystem::buildRigidBodies() {
    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody == nullptr) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);

            PxMaterial *m_material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);
            float halfExtent = .5f;
            physx::PxShape *shape = m_pxPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_material);
            rigidBody.second->m_pxRigidBody = m_pxPhysics->createRigidDynamic(Types::GLMtoPxTransform(transform->getModelMatrix()));
            rigidBody.second->m_pxInitialTransform = Types::GLMtoPxTransform(transform->getModelMatrix());
            rigidBody.second->m_pxRigidBody->attachShape(*shape);
            physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody.second->m_pxRigidBody, 100.0f);
            m_pxScene->addActor(*rigidBody.second->m_pxRigidBody);
            shape->release();
        }
    }
}

void PhysicsSystem::buildCCTs() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        if (component.second->m_CCTController == nullptr) {
            auto *transform = getComponent<TransformComponent>(component.first);

            PxCapsuleControllerDesc desc;
            desc.scaleCoeff = 1;
            desc.position = Types::GLMtoPxExtendedVec3(transform->getTranslation());
            desc.contactOffset = 0.001;
            desc.stepOffset = 0.001;
            desc.slopeLimit = 0;
            desc.radius = component.second->m_radius;
            desc.height = component.second->m_height - (component.second->m_radius * 2.0f);
            desc.upDirection = PxVec3(0, 1, 0);
            desc.material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);
            desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;

            component.second->m_CCTController = m_ControllerManager->createController(desc);
            component.second->setPhysicsPosition(transform->getTranslation());
        }
    }
}

void PhysicsSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &PhysicsSystem::handleEditorUIEvent);
    eventManager->registerEventReceiver(this, &PhysicsSystem::handleInputEvent);
}

bool PhysicsSystem::handleEditorUIEvent(EditorUIEvent *event) {

    if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event->m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }

    return true;
}

bool PhysicsSystem::handleInputEvent(InputEvent *event) {
    for (const auto comp: getComponentContainer<CharacterControllerComponent>()) {
        auto *cameraComp = getComponent<CameraComponent>(comp.first);
        if (cameraComp && cameraComp->m_isActive) {
            processCCTInput(cameraComp, comp.second, event);
        }
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

    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        if (component.second->m_CCTController != nullptr) {
            auto *transform = getComponent<TransformComponent>(component.first);

            transform->resetTransform();
            transform->setTranslation(component.second->m_initialPosition);
            component.second->setPhysicsPosition(component.second->m_initialPosition);
        }
    }
}

void PhysicsSystem::processCCTInput(CameraComponent *camera, CharacterControllerComponent *cct, InputEvent *event) {

    glm::vec3 dir = camera->m_Camera.getViewDirection();
    dir.y = 0;
    dir = glm::normalize(dir);

    glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));

    bool doMove = false;
    PxVec3 direction(0);

    //Up
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 82) {
        direction = direction + Types::GLMtoPxVec3(dir);
        doMove = true;
    }
    //Down
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 81) {
        direction = direction - Types::GLMtoPxVec3(dir);
        doMove = true;
    }
    //Left
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 80) {
        direction = direction - Types::GLMtoPxVec3(right);
        doMove = true;
    }
    //Right
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 79) {
        direction = direction + Types::GLMtoPxVec3(right);
        doMove = true;
    }

    if (doMove) {
        direction.normalize();
        direction = direction * 0.1;

        cct->m_CCTController->move(direction, 0, Time::frameTime, filters);
    }
}

void PhysicsSystem::updateRigidBodies() {
    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody != nullptr) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);
            if (m_isSimulationDisabled) {
                rigidBody.second->m_pxRigidBody->setGlobalPose(Types::GLMtoPxTransform(transform->getModelMatrix()));
                rigidBody.second->m_pxInitialTransform = rigidBody.second->m_pxRigidBody->getGlobalPose();
            } else {
                transform->setFromPxTransform(rigidBody.second->m_pxRigidBody->getGlobalPose());
            }
        }
    }
}

void PhysicsSystem::updateCCTs() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        if (component.second->m_CCTController != nullptr) {
            component.second->m_CCTController->move(PxVec3(0, -0.1, 0), 0, Time::frameTime, filters);

            auto *transform = getComponent<TransformComponent>(component.first);
            if (m_isSimulationDisabled) {
                component.second->setPhysicsPosition(transform->getTranslation());
            } else {
                transform->resetTransform();
                transform->setTranslation(component.second->getPhysicsPosition());

                auto *cameraComp = getComponent<CameraComponent>(component.first);
                if (cameraComp && cameraComp->m_isActive) {
                    cameraComp->m_Camera.setPosition(transform->getTranslation() + glm::vec3(0, component.second->m_height, 0));
                }
            }
        }
    }
}
