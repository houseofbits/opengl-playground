#include "PhysicsSystem.h"
#include "../../../Core/Helper/Time.h"
#include "../../../Core/Helper/Types.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/CharacterControllerComponent.h"
#include "../Components/PhysicsMeshComponent.h"
#include "../Components/RigidBodyComponent.h"

using namespace physx;

static PxControllerFilters filters;

PhysicsSystem::PhysicsSystem() : EntitySystem(),
                                 m_isSimulationDisabled(false),
                                 m_PhysicsResource() {
    usesComponent<RigidBodyComponent>();
    usesComponent<PhysicsMeshComponent>();
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<CharacterControllerComponent>();
}

void PhysicsSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_PhysicsResource, "physics");
}

void PhysicsSystem::process() {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    if (groundPlane == nullptr) {
        PxMaterial *m_material = m_PhysicsResource().m_pxPhysics->createMaterial(0.5, 0.5, 0.5);
        groundPlane = PxCreatePlane(*m_PhysicsResource().m_pxPhysics, physx::PxPlane(0, 1, 0, 0), *m_material);
        m_PhysicsResource().m_pxScene->addActor(*groundPlane);
    }

    buildBodies();
    buildCCTs();

    if (!m_isSimulationDisabled) {
        m_PhysicsResource().m_pxScene->simulate(1.0 / 60.0);
        while (!m_PhysicsResource().m_pxScene->fetchResults(true)) {
        }
    }

    updateBodies();
    updateCCTs();
}

void PhysicsSystem::buildBodies() {
    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody == nullptr) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);

            rigidBody.second->create(*transform);
            m_PhysicsResource().m_pxScene->addActor(*rigidBody.second->m_pxRigidBody);
        }
    }

    for (const auto collisionMesh: getComponentContainer<PhysicsMeshComponent>()) {
        if (collisionMesh.second->m_staticBody == nullptr && collisionMesh.second->m_meshResource.isReady()) {
            auto *transform = getComponent<TransformComponent>(collisionMesh.first);

            collisionMesh.second->create(*transform);
            m_PhysicsResource().m_pxScene->addActor(*collisionMesh.second->m_staticBody);
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
            desc.material = m_PhysicsResource().m_pxPhysics->createMaterial(0.5, 0.5, 0.5);
            desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;

            component.second->m_CCTController = m_PhysicsResource().m_ControllerManager->createController(desc);
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
            transform->m_transform = rigidBody.second->m_initialTransform;
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

void PhysicsSystem::updateBodies() {
    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody != nullptr) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);
            if (m_isSimulationDisabled) {
                rigidBody.second->m_pxRigidBody->setGlobalPose(Types::GLMtoPxTransform(transform->getModelMatrix()));
                rigidBody.second->m_initialTransform = transform->getModelMatrix();
            } else {
                transform->setFromPxTransform(rigidBody.second->m_pxRigidBody->getGlobalPose());
            }
        }
    }

    for (const auto collisionMesh: getComponentContainer<PhysicsMeshComponent>()) {
        if (collisionMesh.second->m_staticBody != nullptr) {
            auto *transform = getComponent<TransformComponent>(collisionMesh.first);
            collisionMesh.second->m_staticBody->setGlobalPose(Types::GLMtoPxTransform(transform->getModelMatrix()));
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
