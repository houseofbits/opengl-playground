#include "CameraComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/CameraControlSystem.h"
#include "../Systems/MainRenderSystem.h"
#include "../Systems/RendererSystem.h"

CameraComponent::CameraComponent() : Component(), m_Camera(), m_isActive(true) {
}
void CameraComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<CameraControlSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}
