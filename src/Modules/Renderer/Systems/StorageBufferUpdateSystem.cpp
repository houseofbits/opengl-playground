#include "StorageBufferUpdateSystem.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/LightComponent.h"

StorageBufferUpdateSystem::StorageBufferUpdateSystem() : EntitySystem(),
                                                         m_LightsBuffer(),
                                                         m_MaterialsBuffer(),
                                                         m_defaultMaterial(),
                                                         m_ResourceManager(nullptr),
                                                         m_ProbesBuffer() {
    m_probeComponentRegistry = useEntityRelatedComponentsRegistry<TransformComponent, EnvironmentProbeComponent>();
    m_lightComponentRegistry = useEntityRelatedComponentsRegistry<TransformComponent, LightComponent>();
}

void StorageBufferUpdateSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    m_ResourceManager = &resourceManager;

    resourceManager.request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager.request(m_ProbesBuffer, "EnvironmentProbeStorageBuffer");
    resourceManager.request(m_MaterialsBuffer, "MaterialsStorageBuffer");
    resourceManager.requestWith(m_defaultMaterial, "defaultMaterial",
                                [&](MaterialResource &resource) {
                                    resource.fetchDefault(resourceManager);
                                });
}

void StorageBufferUpdateSystem::process(EventManager &eventManager) {
    m_LightsBuffer().m_StorageBuffer.reset();

    for (const auto &[id, components]: m_lightComponentRegistry->container()) {
        const auto &[transform, light] = components.get();

        m_LightsBuffer().appendLight(*transform, *light);
    }
    m_LightsBuffer().m_StorageBuffer.updateAll();

    m_ProbesBuffer().m_StorageBuffer.reset();
    for (const auto &[id, components]: m_probeComponentRegistry->container()) {
        const auto &[transform, probe] = components.get();

        m_ProbesBuffer().appendProbe(*transform, *probe);
    }
    m_ProbesBuffer().m_StorageBuffer.updateAll();

    m_MaterialsBuffer().m_StorageBuffer.reset();
    m_MaterialsBuffer().appendMaterial(m_defaultMaterial());
    auto materials = m_ResourceManager->getAllReadyOfType<MaterialResource>();
    for (auto material: materials) {
        m_MaterialsBuffer().appendMaterial(*material);
    }
    m_MaterialsBuffer().m_StorageBuffer.updateAll();
}
