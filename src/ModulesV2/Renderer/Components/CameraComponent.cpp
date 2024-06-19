#include "CameraComponent.h"
#include "../Systems/CameraControlSystem.h"
#include "../Systems/RendererSystem.h"

CameraComponent::CameraComponent() : Component(), m_Camera(), m_isActive(true) {
}
void CameraComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
    ctx.registerComponentWithEntitySystem<CameraControlSystem>(this);
}
