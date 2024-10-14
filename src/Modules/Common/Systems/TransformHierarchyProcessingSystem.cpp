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
    /**
     * 1. On linking calculate depth
     * 2. Sort by depth
     * 3. Process linearly
     */

    std::vector<std::pair<Identity::Type, int>> localTransforms;

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

        if (transformComponent.second->m_parentEntityId > 0) {
            int depth = calculateDepth(transformComponent.first);
            if (depth > 0) {
                localTransforms.emplace_back(transformComponent.first, depth);
            }
        }
    }

    std::sort(localTransforms.begin(), localTransforms.end(), [&](std::pair<Identity::Type, int> &a,
                                                                  std::pair<Identity::Type, int> &b) {
        return a.second < b.second;
    });

    for (const auto ts: localTransforms) {
        auto *transformComponent = getComponent<TransformComponent>(ts.first);
        auto *parentTransformComponent = getComponent<TransformComponent>(transformComponent->m_parentEntityId);

        if (parentTransformComponent != nullptr) {
            transformComponent->updateTransformFromParent(parentTransformComponent->getModelMatrix());
        }
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

int TransformHierarchyProcessingSystem::calculateDepth(Identity::Type entityId) {
    TransformComponent *e = nullptr;
    int d = 0;
    Identity::Type id = entityId;
    do {
        e = getComponent<TransformComponent>(id);
        if (e != nullptr && e->m_parentEntityId > 0) {
            d++;
            id = e->m_parentEntityId;
        }
    } while (e != nullptr && e->m_parentEntityId > 0);

    return d;
}
