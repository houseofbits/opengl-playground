#include "EditorCameraComponent.h"
#include "../../Renderer/Systems/MainRenderSystem.h"
#include "../Systems/EditorUISystem.h"
#include "../Systems/EditorCameraSystem.h"
#include "../../Renderer/Systems/PhysicsDebugRenderSystem.h"

EditorCameraComponent::EditorCameraComponent() : Component(),
                                                 BaseCameraComponent() {

}

void EditorCameraComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<EditorCameraSystem>(this);
}

void EditorCameraComponent::serialize(nlohmann::json &j) {
    j["position"] = m_Camera.getPosition();
    j["viewDirection"] = m_Camera.getViewDirection();
    j["upDirection"] = m_Camera.getUpDirection();
    j["isActive"] = m_isActive;
}

void EditorCameraComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    glm::vec3 position = j.value("position", glm::vec3(0, 0, 0));
    glm::vec3 view = j.value("viewDirection", glm::vec3(0, 0, 1));
    glm::vec3 up = j.value("upDirection", glm::vec3(0, 1, 0));

    m_Camera.setPosition(position)
            .setView(view, up)
            .setViewportSize(1920, 1080)
            .setFieldOfView(90);

    m_isActive = j.value("isActive", false);
}

void EditorCameraComponent::rotateView(glm::vec2 viewChangeAlongScreenAxis) {
    auto view = m_Camera.getViewDirection();

    glm::vec3 rightHorizontal = glm::normalize(glm::cross(view, glm::vec3(0, 1, 0)));
    glm::vec3 upVertical = glm::normalize(glm::cross(view, rightHorizontal));

    glm::vec3 viewChange = rightHorizontal * viewChangeAlongScreenAxis.x + upVertical * viewChangeAlongScreenAxis.y;
    view = glm::normalize(view + viewChange);
    glm::vec3 right = glm::normalize(glm::cross(view, glm::vec3(0, 1, 0)));

    m_Camera.setView(view, glm::normalize(glm::cross(right, view)));
}

void EditorCameraComponent::moveView(glm::vec3 direction) {
    auto position = m_Camera.getPosition();
    auto transform = m_Camera.getTransformMatrix();

    position = position
               + glm::vec3(transform[2]) * direction.z
               + glm::vec3(transform[1]) * direction.y
               + glm::vec3(transform[0]) * direction.x;

    m_Camera.setPosition(glm::vec4(position, 1.0));
}