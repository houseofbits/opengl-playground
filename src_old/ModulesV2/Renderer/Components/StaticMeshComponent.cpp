#include "StaticMeshComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/EnvironmentProbeRenderSystem.h"
#include "../Systems/MainRenderSystem.h"
#include "../Systems/ShadowMapRenderSystem.h"

StaticMeshComponent::StaticMeshComponent() : Component(), m_Mesh(), m_Material() {
}

void StaticMeshComponent::registerWithSystems(EntityContext& ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EnvironmentProbeRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<ShadowMapRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}