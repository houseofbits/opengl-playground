#include "TransformHierarchyProcessingSystem.h"
#include "../Components/CameraComponent.h"
#include "../Components/TransformComponent.h"

TransformHierarchyProcessingSystem::TransformHierarchyProcessingSystem() : EntitySystem() {
    usesComponent<CameraComponent>();
    usesComponent<TransformComponent>();
}

void TransformHierarchyProcessingSystem::initialize(ResourceManager &) {

}

void TransformHierarchyProcessingSystem::process(EventManager &) {
    for (const auto &transformComponent: getComponentContainer<TransformComponent>()) {
        if (transformComponent.second->m_shouldUpdateParentEntityId) {
            if (transformComponent.second->m_parentEntityName.empty()) {
                transformComponent.second->m_parentEntityId = 0;
                transformComponent.second->m_shouldUpdateParentEntityId = false;

                continue;
            }

            auto a = m_EntityContext->findEntity(transformComponent.second->m_parentEntityName);
            if (a != nullptr) {
                transformComponent.second->m_parentEntityId = a->m_Id.id();
            } else {
                Log::warn("Could not find parent entity ", transformComponent.second->m_parentEntityName);
            }
            transformComponent.second->m_shouldUpdateParentEntityId = false;
        }

        //TODO Calculate world space transform
    }

    for (const auto &cameraComponent: getComponentContainer<CameraComponent>()) {
        auto *transformComponent = getComponent<TransformComponent>(cameraComponent.first);
        if (transformComponent != nullptr) {
            cameraComponent.second->updateTransformFromParent(transformComponent->m_transform);
        } else {
            cameraComponent.second->updateTransformWorld();
        }
    }
}
