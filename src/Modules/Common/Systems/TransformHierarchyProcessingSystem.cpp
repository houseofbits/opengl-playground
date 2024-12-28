#include "TransformHierarchyProcessingSystem.h"

TransformHierarchyProcessingSystem::TransformHierarchyProcessingSystem() : EntitySystem() {
    m_transformComponentRegistry = useComponentRegistry<TransformComponent>();
    m_cameraComponentRegistry = useComponentRegistry<CameraComponent>();
}

void TransformHierarchyProcessingSystem::initialize(ResourceManager &, EventManager&) {
}

void TransformHierarchyProcessingSystem::process(EventManager &) {
    /**
     * 1. On linking calculate depth
     * 2. Sort by depth
     * 3. Process linearly
     */

    std::vector<std::pair<Identity::Type, int> > localTransforms;

    for (const auto [id, component]: m_transformComponentRegistry->container()) {
        if (component->isLinkedToEntityId()) {
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

        if (auto *parentTransformComponent = m_transformComponentRegistry->get(transformComponent->getLinkedEntityId());
            parentTransformComponent != nullptr) {
            transformComponent->updateTransformFromParent(parentTransformComponent->getModelMatrix());
        } else {
            transformComponent->updateTransformWorld();
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

int TransformHierarchyProcessingSystem::calculateDepth(Identity::Type entityId) const {
    TransformComponent *e = nullptr;
    int d = 0;
    Identity::Type id = entityId;
    do {
        e = m_transformComponentRegistry->get(id);
        if (e != nullptr && e->isLinkedToEntityId()) {
            d++;
            id = e->getLinkedEntityId();
        }
    } while (e != nullptr && e->isLinkedToEntityId());

    return d;
}
