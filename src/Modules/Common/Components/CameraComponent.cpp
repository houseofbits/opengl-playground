#include "CameraComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../../Physics/Systems/PhysicsSystem.h"
#include "../../Renderer/Systems/MainRenderSystem.h"
#include "../Systems/CameraControlSystem.h"

CameraComponent::CameraComponent() : Component(), m_Camera(), m_isActive(true) {
}
void CameraComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<CameraControlSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<PhysicsSystem>(this);
}
