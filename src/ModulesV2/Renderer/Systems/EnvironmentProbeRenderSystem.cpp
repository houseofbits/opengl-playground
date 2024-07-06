#include "EnvironmentProbeRenderSystem.h"
#include "../Components/CameraComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/StaticMeshComponent.h"
#include <GL/glew.h>

EnvironmentProbeRenderSystem::EnvironmentProbeRenderSystem() : EntitySystem(),
                                                               m_ShaderProgram(),
                                                               m_LightsBuffer(),
                                                               m_SamplersIndexBuffer() {
    usesComponent<StaticMeshComponent>();
    usesComponent<TransformComponent>();
    usesComponent<EnvironmentProbeComponent>();
}

void EnvironmentProbeRenderSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_LightsBuffer, "SpotLightStorageBuffer");
    resourceManager->request(m_SamplersIndexBuffer, "SamplerIndexStorageBuffer");
}

void EnvironmentProbeRenderSystem::process() {
}
