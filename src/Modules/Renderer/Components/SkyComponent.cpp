#include "SkyComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/EnvironmentProbeRenderSystem.h"
#include "../Systems/MainRenderSystem.h"

SkyComponent::SkyComponent() : Component(), m_cubeMap() {
}

void SkyComponent::registerWithSystems(EntityContext& ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EnvironmentProbeRenderSystem>(this);
}