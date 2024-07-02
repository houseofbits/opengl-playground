#include "StaticMeshComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/RendererSystem.h"

StaticMeshComponent::StaticMeshComponent() : Component(), m_Mesh(), m_Material() {
}

void StaticMeshComponent::registerWithSystems(EntityContext& ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}