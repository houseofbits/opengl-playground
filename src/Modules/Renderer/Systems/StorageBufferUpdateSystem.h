#pragma once

#include "../../../Core/API.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/MaterialsBufferResource.h"

class StorageBufferUpdateSystem : public EntitySystem {
public:
    StorageBufferUpdateSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager&) override;

    ResourceManager* m_ResourceManager;
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<MaterialsBufferResource> m_MaterialsBuffer;
    ResourceHandle<EnvironmentProbesBufferResource> m_ProbesBuffer;
    EntityRelatedComponentRegistry<TransformComponent, EnvironmentProbeComponent>* m_probeComponentRegistry;
    EntityRelatedComponentRegistry<TransformComponent, LightComponent>* m_lightComponentRegistry;
    ResourceHandle<MaterialResource> m_defaultMaterial;
};
