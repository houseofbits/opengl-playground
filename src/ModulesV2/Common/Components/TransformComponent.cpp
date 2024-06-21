#include "TransformComponent.h"
#include "../../Renderer/Systems/RendererSystem.h"

TransformComponent::TransformComponent() : m_ModelMatrix(1.0), m_Translation(0.0), m_Rotation(0.0), m_Scale(1.0) {
}

void TransformComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
}
