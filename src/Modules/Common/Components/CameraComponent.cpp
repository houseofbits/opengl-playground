#include "CameraComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../Physics/Systems/CharacterControllerSystem.h"
#include "../../Physics/Systems/PhysicsSystem.h"
#include "../../Renderer/Systems/MainRenderSystem.h"

CameraComponent::CameraComponent() : Component(),
                                     m_Camera(),
                                     m_isActive(true),
                                     m_type(TYPE_FREE) {
}

void CameraComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<MainRenderSystem>(this);
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

void CameraComponent::serialize(nlohmann::json &j) {
    j["position"] = m_Camera.getPosition();
    j["viewDirection"] = m_Camera.getViewDirection();
    j["upDirection"] = m_Camera.getUpDirection();
    j["isActive"] = m_isActive;
    j["type"] = m_TypeNameMap[m_type];
}

void CameraComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    glm::vec3 position = j.value("position", glm::vec3(0, 0, 0));
    glm::vec3 view = j.value("viewDirection", glm::vec3(0, 0, 1));
    glm::vec3 up = j.value("upDirection", glm::vec3(0, 1, 0));

    m_Camera.setPosition(position)
            .setView(view, up)
            .setViewportSize(1920, 1080)
            .setFieldOfView(90);

    m_isActive = j.value("isActive", false);
    m_type = getTypeFromName(j.value("type", m_TypeNameMap.begin()->second));
}

CameraComponent::Type CameraComponent::getTypeFromName(const std::string &name) {
    for (const auto &[key, value]: m_TypeNameMap) {
        if (value == name) {
            return key;
        }
    }

    return TYPE_FREE;
}

void CameraComponent::rotateView(glm::vec2 viewChangeAlongScreenAxis) {
    TBN tbn = calculateTBN(m_Camera.getViewDirection());

    glm::vec3 rightHorizontal = glm::normalize(glm::cross(tbn.view, glm::vec3(0, 1, 0)));
    glm::vec3 upVertical = glm::normalize(glm::cross(tbn.view, rightHorizontal));

    glm::vec3 viewChange = rightHorizontal * viewChangeAlongScreenAxis.x;
    tbn = calculateTBN(tbn.view + viewChange);

    viewChange = upVertical * viewChangeAlongScreenAxis.y;
    tbn = calculateTBN(tbn.view + viewChange);

    m_Camera.setView(tbn.view, tbn.up);
}
