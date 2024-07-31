#pragma once

#include "../../../CoreV2/API.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/LightsBufferResource.h"

class StorageBufferUpdateSystem : public EntitySystem {
public:
    StorageBufferUpdateSystem();

    void process() override;
    void initialize(ResourceManager*) override;

    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<EnvironmentProbesBufferResource> m_ProbesBuffer;
};
