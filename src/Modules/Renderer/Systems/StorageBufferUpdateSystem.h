#pragma once

#include "../../../Core/API.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/LightsBufferResource.h"

class StorageBufferUpdateSystem : public EntitySystem {
public:
    StorageBufferUpdateSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager&) override;

    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<EnvironmentProbesBufferResource> m_ProbesBuffer;
    RelatedComponentRegistry<TransformComponent, EnvironmentProbeComponent>* m_probeComponentRegistry;
    RelatedComponentRegistry<TransformComponent, LightComponent>* m_lightComponentRegistry;
};
