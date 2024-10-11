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
    for (const auto &cameraComponent: getComponentContainer<CameraComponent>()) {
        auto *transformComponent = getComponent<TransformComponent>(cameraComponent.first);
        if (transformComponent != nullptr) {
            cameraComponent.second->updateTransformFromParent(transformComponent->m_transform);
        } else {
            cameraComponent.second->updateTransformWorld();
        }
    }
}
