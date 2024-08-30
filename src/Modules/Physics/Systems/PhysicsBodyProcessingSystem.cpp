#include "PhysicsBodyProcessingSystem.h"
#include "../../../Core/Helper/Types.h"
#include "../Components/PhysicsMeshComponent.h"
#include "../Components/RigidBodyComponent.h"

PhysicsBodyProcessingSystem::PhysicsBodyProcessingSystem() : EntitySystem(),
                                                             m_PhysicsResource(),
                                                             m_isSimulationDisabled(false) {
    usesComponent<RigidBodyComponent>();
    usesComponent<PhysicsMeshComponent>();
    usesComponent<TransformComponent>();
}

void PhysicsBodyProcessingSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_PhysicsResource, "physics");
}

void PhysicsBodyProcessingSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &PhysicsBodyProcessingSystem::handleEditorUIEvent);
}

void PhysicsBodyProcessingSystem::process() {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    updateRigidBodyTransforms();
    updateStaticBodyTransforms();
    createRigidBodies();
    createStaticBodies();
}

void PhysicsBodyProcessingSystem::createRigidBodies() {
    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody == nullptr && rigidBody.second->m_meshResource.isReady()) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);
            rigidBody.second->create(*transform);
            m_PhysicsResource().m_pxScene->addActor(*rigidBody.second->m_pxRigidBody);
        }
    }
}

void PhysicsBodyProcessingSystem::createStaticBodies() {
    for (const auto collisionMesh: getComponentContainer<PhysicsMeshComponent>()) {
        if (collisionMesh.second->m_staticBody == nullptr && collisionMesh.second->m_meshResource.isReady()) {
            auto *transform = getComponent<TransformComponent>(collisionMesh.first);
            collisionMesh.second->create(*transform);
            m_PhysicsResource().m_pxScene->addActor(*collisionMesh.second->m_staticBody);
        }
    }
}

void PhysicsBodyProcessingSystem::updateRigidBodyTransforms() {
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
}

void PhysicsBodyProcessingSystem::updateStaticBodyTransforms() {
    for (const auto collisionMesh: getComponentContainer<PhysicsMeshComponent>()) {
        if (collisionMesh.second->m_staticBody != nullptr) {
            auto *transform = getComponent<TransformComponent>(collisionMesh.first);
            collisionMesh.second->m_staticBody->setGlobalPose(Types::GLMtoPxTransform(transform->getModelMatrix()));
        }
    }
}

bool PhysicsBodyProcessingSystem::handleEditorUIEvent(EditorUIEvent *event) {
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

void PhysicsBodyProcessingSystem::resetToInitialTransform() {
    for (const auto rigidBody: getComponentContainer<RigidBodyComponent>()) {
        if (rigidBody.second->m_pxRigidBody != nullptr) {
            auto *transform = getComponent<TransformComponent>(rigidBody.first);
            transform->m_transform = rigidBody.second->m_initialTransform;
        }
    }
}
