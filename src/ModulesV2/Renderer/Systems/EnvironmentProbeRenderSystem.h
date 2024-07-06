#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/SamplersIndexBufferResource.h"
#include "../Resources/ShaderProgramResource.h"

class EnvironmentProbeRenderSystem : public EntitySystem {
public:
    EnvironmentProbeRenderSystem();

    void process() override;
    void initialize(ResourceManager *) override;

private:
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<SamplersIndexBufferResource> m_SamplersIndexBuffer;
};
