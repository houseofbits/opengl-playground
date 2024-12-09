#include "TransformHierarchyProcessingSystem.h"

TransformHierarchyProcessingSystem::TransformHierarchyProcessingSystem() : EntitySystem() {
    m_transformComponentRegistry = useComponentRegistry<TransformComponent>();
    m_cameraComponentRegistry = useComponentRegistry<CameraComponent>();
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

    for (const auto [id, component]: m_transformComponentRegistry->container()) {
        if (component->m_shouldUpdateParentEntityId) {
            if (component->m_parentEntityName.empty()) {
                component->m_parentEntityId = 0;
                component->m_shouldUpdateParentEntityId = false;

                continue;
            }
            auto a = m_EntityContext->findEntity(component->m_parentEntityName);
            if (a != nullptr) {
                component->m_parentEntityId = a->m_Id.id();
            } else {
                Log::warn("Could not find parent entity ", component->m_parentEntityName);
            }
            component->m_shouldUpdateParentEntityId = false;
        }

        if (component->m_parentEntityId > 0) {
            int depth = calculateDepth(id);
            if (depth > 0) {
                localTransforms.emplace_back(id, depth);
            }
        }
    }

    std::sort(localTransforms.begin(), localTransforms.end(), [&](std::pair<Identity::Type, int> &a,
                                                                  std::pair<Identity::Type, int> &b) {
        return a.second < b.second;
    });

    for (const auto ts: localTransforms) {
        auto *transformComponent = m_transformComponentRegistry->get(ts.first);
        auto *parentTransformComponent = m_transformComponentRegistry->get(transformComponent->m_parentEntityId);

        if (parentTransformComponent != nullptr) {
            transformComponent->updateTransformFromParent(parentTransformComponent->getModelMatrix());
        }
    }

    for (const auto [id, component]: m_cameraComponentRegistry->container()) {
        auto *transformComponent = m_transformComponentRegistry->get(id);
        if (transformComponent != nullptr) {
            component->updateTransformFromParent(transformComponent->m_transform);
        } else {
            component->updateTransformWorld();
        }
    }
}

int TransformHierarchyProcessingSystem::calculateDepth(Identity::Type entityId) {
    TransformComponent *e = nullptr;
    int d = 0;
    Identity::Type id = entityId;
    do {
        e = m_transformComponentRegistry->get(id);
        if (e != nullptr && e->m_parentEntityId > 0) {
            d++;
            id = e->m_parentEntityId;
        }
    } while (e != nullptr && e->m_parentEntityId > 0);

    return d;
}
