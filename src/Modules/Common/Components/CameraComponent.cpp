#include "CameraComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"

CameraComponent::CameraComponent() : Component(),
                                     ComponentTransformEdit(),
                                     BaseCameraComponent(),
                                     m_type(TYPE_FREE),
                                     m_initialTransformLocal(1.0),
                                     m_currentTransformWorld(1.0),
                                     m_shouldSyncWorldTransformToLocal(false) {
}

void CameraComponent::registerWithSystems(EntityContext &ctx) {

}

void CameraComponent::serialize(nlohmann::json &j) {
    j["position"] = glm::vec3(m_initialTransformLocal[3]);
    j["viewDirection"] = glm::vec3(m_initialTransformLocal[2]);
    j["upDirection"] = glm::vec3(m_initialTransformLocal[1]);

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

    m_initialTransformLocal = glm::mat4(1.0f);
    m_initialTransformLocal[0] = glm::vec4(glm::normalize(glm::cross(view, up)), 0.0f);
    m_initialTransformLocal[1] = glm::vec4(glm::normalize(up), 0.0f);
    m_initialTransformLocal[2] = glm::vec4(glm::normalize(view), 0.0f);
    m_initialTransformLocal[3] = glm::vec4(position, 1.0);
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
    auto view = glm::vec3(m_initialTransformLocal[2]);

    glm::vec3 rightHorizontal = glm::normalize(glm::cross(view, glm::vec3(0, 1, 0)));
    glm::vec3 upVertical = glm::normalize(glm::cross(view, rightHorizontal));

    glm::vec3 viewChange = rightHorizontal * viewChangeAlongScreenAxis.x + upVertical * viewChangeAlongScreenAxis.y;
    view = glm::normalize(view + viewChange);
    glm::vec3 right = glm::normalize(glm::cross(view, glm::vec3(0, 1, 0)));

    m_initialTransformLocal[0] = glm::vec4(right, 0.0f);
    m_initialTransformLocal[1] = glm::vec4(glm::normalize(glm::cross(right, view)), 0.0f);
    m_initialTransformLocal[2] = glm::vec4(view, 0.0f);
}

glm::mat4 CameraComponent::getEditorTransform() {
    return m_currentTransformWorld;
}

void CameraComponent::setFromEditorTransform(const glm::mat4 &m) {
    m_currentTransformWorld = m;
    m_shouldSyncWorldTransformToLocal = true;
}

void CameraComponent::updateTransformFromParent(const glm::mat4 &parent) {
    if (m_shouldSyncWorldTransformToLocal) {
        m_initialTransformLocal = glm::inverse(parent) * m_currentTransformWorld;
        m_shouldSyncWorldTransformToLocal = false;
    } else {
        m_currentTransformWorld = parent * m_initialTransformLocal;
    }

    m_Camera.setFromTransformMatrix(m_currentTransformWorld);
}

void CameraComponent::updateTransformWorld() {
    if (m_shouldSyncWorldTransformToLocal) {
        m_initialTransformLocal = m_currentTransformWorld;
        m_shouldSyncWorldTransformToLocal = false;
    } else {
        m_currentTransformWorld = m_initialTransformLocal;
    }

    m_Camera.setFromTransformMatrix(m_currentTransformWorld);
}

void CameraComponent::moveView(glm::vec3 direction) {
    auto position = glm::vec3(m_initialTransformLocal[3]);

    position = position
               + glm::vec3(m_initialTransformLocal[2]) * direction.z
               + glm::vec3(m_initialTransformLocal[1]) * direction.y
               + glm::vec3(m_initialTransformLocal[0]) * direction.x;

    m_initialTransformLocal[3] = glm::vec4(position, 1.0);
}
