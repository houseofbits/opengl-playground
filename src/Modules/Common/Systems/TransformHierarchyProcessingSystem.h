#pragma once

#include "../../../Core/API.h"
#include "../Components/CameraComponent.h"
#include "../Components/TransformComponent.h"

class TransformHierarchyProcessingSystem : public EntitySystem {
public:
    TransformHierarchyProcessingSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

private:
    int calculateDepth(Identity::Type entityId) const;

    SingleComponentRegistry<TransformComponent>* m_transformComponentRegistry;
    SingleComponentRegistry<CameraComponent>* m_cameraComponentRegistry;
};
