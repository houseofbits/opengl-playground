#include "TransformComponent.h"
#include "../../Renderer/Systems/RendererSystem.h"

TransformComponent::TransformComponent() : m_Translation(), m_Rotation(), m_Scale() {
}

void TransformComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
}
