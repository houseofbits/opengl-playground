#include "MaterialComponent.h"
#include "../Systems/RendererSystem.h"

MaterialComponent::MaterialComponent() : Component(), m_Diffuse() {
}

void MaterialComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
}
