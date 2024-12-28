#include "StorageBufferUpdateSystem.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/LightComponent.h"

StorageBufferUpdateSystem::StorageBufferUpdateSystem() : EntitySystem(),
                                                         m_LightsBuffer(),
                                                         m_ProbesBuffer() {
    m_probeComponentRegistry = useComponentsRegistry<TransformComponent, EnvironmentProbeComponent>();
    m_lightComponentRegistry = useComponentsRegistry<TransformComponent, LightComponent>();
}

void StorageBufferUpdateSystem::initialize(ResourceManager &resourceManager, EventManager&) {
    resourceManager.request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager.request(m_ProbesBuffer, "EnvironmentProbeStorageBuffer");
//
//    std::cout<<sizeof(std::uint64_t)<<std::endl;
//    std::cout<<sizeof(glm::mat4)<<std::endl;
//    std::cout<<sizeof(LightsBufferResource::LightStructure)<<std::endl;
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
}
