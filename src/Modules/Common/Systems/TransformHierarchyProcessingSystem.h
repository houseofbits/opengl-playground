#pragma once

#include "../../../Core/API.h"
#include "../Components/CameraComponent.h"
#include "../Components/TransformComponent.h"

class TransformHierarchyProcessingSystem : public EntitySystem {
public:
    TransformHierarchyProcessingSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

private:
    int calculateDepth(Identity::Type entityId) const;

    EntityUniqueComponentRegistry<TransformComponent>* m_transformComponentRegistry;
    EntityUniqueComponentRegistry<CameraComponent>* m_cameraComponentRegistry;
};
