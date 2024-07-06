#include "StorageBufferUpdateSystem.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/LightComponent.h"

StorageBufferUpdateSystem::StorageBufferUpdateSystem() : EntitySystem(),
                                                         m_LightsBuffer(),
                                                         m_ProbesBuffer(),
                                                         m_SamplersIndexBuffer() {
    usesComponent<LightComponent>();
    usesComponent<TransformComponent>();
    usesComponent<EnvironmentProbeComponent>();
}

void StorageBufferUpdateSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager->request(m_ProbesBuffer, "EnvironmentProbeStorageBuffer");
    resourceManager->request(m_SamplersIndexBuffer, "SamplerIndexStorageBuffer");
}

void StorageBufferUpdateSystem::process() {
    m_LightsBuffer().m_StorageBuffer.reset();
    m_SamplersIndexBuffer().m_StorageBuffer.reset();
    for (const auto &light: getComponentContainer<LightComponent>()) {
        auto *transform = getComponent<TransformComponent>(light.second->m_EntityId.id());
        if (transform == nullptr) {
            continue;
        }

        int index = -1;
        //TODO Sampler index buffer can be populated at the texture resource build time.
        // so you could just call index = light.second->m_Projection().m_samplerIndex;
        if (light.second->m_Projection.isValid()) {
            index = m_SamplersIndexBuffer().getOrAppend(light.second->m_Projection().m_handleId);
        }

        m_LightsBuffer().appendLight(*transform, *light.second, index);
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
