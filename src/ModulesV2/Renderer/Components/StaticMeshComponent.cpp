#include "StaticMeshComponent.h"
#include "../Systems/RendererSystem.h"

StaticMeshComponent::StaticMeshComponent() : Component(), m_Mesh() {
}

void StaticMeshComponent::registerWithSystems(EntityContext& ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
}