#include "EditorCameraComponent.h"
#include "../../Renderer/Systems/MainRenderSystem.h"
#include "../Systems/EditorUISystem.h"

EditorCameraComponent::EditorCameraComponent() : Component(),
                                                 BaseCameraComponent(),
                                                 m_type(TYPE_PERSPECTIVE),
                                                 m_orthographicScale(1.0) {
}

void EditorCameraComponent::serialize(nlohmann::json &j) {
    j[POSITION_KEY] = m_Camera.getPosition();
    j[VIEW_KEY] = m_Camera.getViewDirection();
    j[UP_KEY] = m_Camera.getUpDirection();
    j[ACTIVE_KEY] = m_isActive;
    j[TYPE_KEY] = m_type;
    j[SCALE_KEY] = m_orthographicScale;
}

void EditorCameraComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    glm::vec3 position = j.value(POSITION_KEY, glm::vec3(0, 0, 0));
    glm::vec3 view = j.value(VIEW_KEY, glm::vec3(0, 0, 1));
    glm::vec3 up = j.value(UP_KEY, glm::vec3(0, 1, 0));

    m_Camera.setPosition(position)
            .setView(view, up)
            .setViewportSize(1920, 1080)
            .setFieldOfView(90);

    m_isActive = j.value(ACTIVE_KEY, false);
    m_type = j.value(TYPE_KEY, m_type);
    m_orthographicScale = j.value(SCALE_KEY, m_orthographicScale);

    if (m_type == TYPE_PERSPECTIVE) {
        m_Camera.setPerspective();
    } else {
        m_Camera.setOrthographic(m_orthographicScale);
    }
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

void EditorCameraComponent::setOrthographic(bool isOrthographic) {
    if (isOrthographic) {
        m_type = TYPE_ORTHOGRAPHIC;
        m_Camera.setOrthographic(m_orthographicScale);
    } else {
        m_type = TYPE_PERSPECTIVE;
        m_Camera.setPerspective();
    }
}

void EditorCameraComponent::setOrthographicScale(float scale) {
    m_orthographicScale = scale;

    setOrthographic(m_type == TYPE_ORTHOGRAPHIC);
}

void EditorCameraComponent::setOrthographicViewType(OrthographicViewType type) {
    glm::vec3 pos = -OrthographicViews[type].first * 20.0f;

    m_Camera
            .setView(OrthographicViews[type].first, OrthographicViews[type].second)
            .setPosition(pos);
}

void EditorCameraComponent::setOrthographicViewTypeFocused(OrthographicViewType type, TransformComponent &transform) {
    //TODO: When we'll have entity bounding boxes - adjust camera scale to zoom in on the selected object
    const glm::vec3 pos = transform.getWorldPosition() - (OrthographicViews[type].first * 20.0f);

    m_Camera
            .setView(OrthographicViews[type].first, OrthographicViews[type].second)
            .setPosition(pos);
}