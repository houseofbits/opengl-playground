#pragma once

#include "../../../CoreV2/API.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/SamplersIndexBufferResource.h"

class StorageBufferUpdateSystem : public EntitySystem {
public:
    StorageBufferUpdateSystem();

    void process() override;
    void initialize(ResourceManager*) override;

    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<EnvironmentProbesBufferResource> m_ProbesBuffer;
    ResourceHandle<SamplersIndexBufferResource> m_SamplersIndexBuffer;
};
