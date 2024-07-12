#include "StorageBufferUpdateSystem.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/LightComponent.h"

StorageBufferUpdateSystem::StorageBufferUpdateSystem() : EntitySystem(),
                                                         m_LightsBuffer(),
                                                         m_ProbesBuffer() {
    usesComponent<LightComponent>();
    usesComponent<TransformComponent>();
    usesComponent<EnvironmentProbeComponent>();
}

void StorageBufferUpdateSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager->request(m_ProbesBuffer, "EnvironmentProbeStorageBuffer");
//
//    std::cout<<sizeof(std::uint64_t)<<std::endl;
//    std::cout<<sizeof(glm::mat4)<<std::endl;
//    std::cout<<sizeof(LightsBufferResource::LightStructure)<<std::endl;
}

void StorageBufferUpdateSystem::process() {
    m_LightsBuffer().m_StorageBuffer.reset();
    for (const auto &light: getComponentContainer<LightComponent>()) {
        auto *transform = getComponent<TransformComponent>(light.second->m_EntityId.id());
        if (transform == nullptr || !light.second->m_isEnabled) {
            continue;
        }

        m_LightsBuffer().appendLight(*transform, *light.second);
    }
    m_LightsBuffer().m_StorageBuffer.updateAll();

    m_ProbesBuffer().m_StorageBuffer.reset();
    for (const auto &probe: getComponentContainer<EnvironmentProbeComponent>()) {
        auto *transform = getComponent<TransformComponent>(probe.second->m_EntityId.id());
        if (transform == nullptr) {
            continue;
        }
        m_ProbesBuffer().appendProbe(*transform, *probe.second);
    }
    m_ProbesBuffer().m_StorageBuffer.updateAll();
}
