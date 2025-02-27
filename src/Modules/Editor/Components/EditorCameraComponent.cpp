#include "EditorCameraComponent.h"
#include "../../Renderer/Systems/MainRenderSystem.h"
#include "../Systems/EditorUISystem.h"

EditorCameraComponent::EditorCameraComponent() : Component(),
                                                 BaseCameraComponent(),
                                                 m_type(TYPE_ORTHOGRAPHIC),
                                                 m_orthographicScale(1.0),
                                                 m_cameraCenter(0, 0, 0) {
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

    m_cameraCenter = position + view * 10.0f;
}

void EditorCameraComponent::rotateView(const glm::vec2 viewChangeAlongScreenAxis) {
    auto view = m_Camera.getViewDirection();

    const glm::vec3 rightHorizontal = glm::normalize(glm::cross(view, glm::vec3(0, 1, 0)));
    const glm::vec3 upVertical = glm::normalize(glm::cross(view, rightHorizontal));

    const glm::vec3 viewChange = rightHorizontal * viewChangeAlongScreenAxis.x + upVertical * viewChangeAlongScreenAxis.
                                 y;
    view = glm::normalize(view + viewChange);
    const glm::vec3 right = glm::normalize(glm::cross(view, glm::vec3(0, 1, 0)));

    const glm::vec3 up = glm::normalize(glm::cross(right, view));

    m_Camera.setFromViewTarget(m_cameraCenter, view * 10.0f, up);
}

void EditorCameraComponent::moveView(const glm::vec3 direction) {
    auto position = m_Camera.getPosition();
    auto transform = m_Camera.getTransformMatrix();

    const glm::vec3 translation = glm::vec3(transform[2]) * direction.z
                                  + glm::vec3(transform[1]) * direction.y
                                  + glm::vec3(transform[0]) * direction.x;

    m_cameraCenter = m_cameraCenter + translation;
    position = position + translation;

    m_Camera.setPosition(glm::vec4(position, 1.0));
}

void EditorCameraComponent::setOrthographic(const bool isOrthographic) {
    if (isOrthographic) {
        m_type = TYPE_ORTHOGRAPHIC;
        m_Camera.setOrthographic(m_orthographicScale);
    } else {
        m_type = TYPE_PERSPECTIVE;
        m_Camera.setPerspective();
    }
}

void EditorCameraComponent::setOrthographicScale(const float scale) {
    m_orthographicScale = scale;

    setOrthographic(m_type == TYPE_ORTHOGRAPHIC);
}

void EditorCameraComponent::setOrthographicViewType(const OrthographicViewType type) {
    m_cameraCenter = -OrthographicViews[type].first * 20.0f;
    m_Camera.setFromViewTarget(m_cameraCenter, OrthographicViews[type].first, OrthographicViews[type].second);
}

void EditorCameraComponent::setOrthographicViewTypeFocused(const OrthographicViewType type, TransformComponent &transform) {
    //TODO: When we'll have entity bounding boxes - adjust camera scale to zoom in on the selected object
    m_cameraCenter = transform.getWorldPosition() - (OrthographicViews[type].first * 20.0f);
    m_Camera.setFromViewTarget(m_cameraCenter, OrthographicViews[type].first * 10.0f, OrthographicViews[type].second);
}

void EditorCameraComponent::setViewTarget(glm::vec3 target) {
    m_cameraCenter = target;
    rotateView({0,0});
}