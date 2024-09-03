#include "CameraComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../../Physics/Systems/CharacterControllerSystem.h"
#include "../../Physics/Systems/PhysicsSystem.h"
#include "../../Renderer/Systems/MainRenderSystem.h"
#include "../Systems/CameraControlSystem.h"

CameraComponent::CameraComponent() : Component(),
                                     m_Camera(),
                                     m_isActive(true),
                                     m_type(TYPE_FREE) {
}

void CameraComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
    ctx.registerComponentWithEntitySystem<CameraControlSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<PhysicsSystem>(this);
    ctx.registerComponentWithEntitySystem<CharacterControllerSystem>(this);
}

CameraComponent::TBN CameraComponent::calculateTBN(glm::vec3 viewDirection) {
    TBN tbn{};

    tbn.view = glm::normalize(viewDirection);
    tbn.right = glm::normalize(glm::cross(tbn.view, glm::vec3(0, 1, 0)));
    tbn.up = glm::normalize(glm::cross(tbn.right, viewDirection));

    return tbn;
}
