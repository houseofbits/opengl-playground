#include "StaticMeshComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Systems/EnvironmentProbeRenderSystem.h"
#include "../Systems/MainRenderSystem.h"
#include "../Systems/ShadowMapRenderSystem.h"

StaticMeshComponent::StaticMeshComponent() : Component(), m_Mesh(), m_Material(), m_targetRenderer(SOLID) {
    m_TargetRenderNameMap = {
        {SOLID, "SOLID"},
        {SHADOW, "SHADOW_ONLY"}
    };
}

void StaticMeshComponent::registerWithSystems(EntityContext& ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EnvironmentProbeRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<ShadowMapRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}