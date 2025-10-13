#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "BaseCameraComponent.h"
#include "../../Renderer/Resources/RenderTargetResource.h"

class CameraComponent : public Component, public BaseCameraComponent {
    friend class CameraComponentTransformEdit;

    TYPE_DEFINITION(CameraComponent);
    inline static const std::string POSITION_KEY = "position";
    inline static const std::string VIEW_KEY = "viewDirection";
    inline static const std::string UP_KEY = "upDirection";
    inline static const std::string REL_ROTATION_KEY = "disableRelRotation";
    inline static const std::string ACTIVE_KEY = "isActive";
    inline static const std::string IS_RENDER_TARGET_KEY = "isRenderTarget";
    inline static const std::string RENDER_TARGET_WIDTH_KEY = "renderTargetWidth";
    inline static const std::string RENDER_TARGET_HEIGHT_KEY = "renderTargetHeight";

public:
    CameraComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void rotateView(glm::vec2 viewChangeAlongScreenAxis);

    void moveView(glm::vec3 direction);

    glm::mat4 getWorldTransform();

    void setWorldTransform(const glm::mat4 &);

    void updateTransformFromParent(const glm::mat4 &parent);

    void updateTransformWorld();

    glm::vec3 getWorldPosition() {
        return m_currentTransformWorld[3];
    }

    void setPositionAndDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

    [[nodiscard]] std::string getRenderTargetResourceName() const {
        if (m_Name.empty()) {
            return "camera-render-target";
        }

        return m_Name + "-render-target";
    }

    bool m_isRelativeRotationDisabled;;
    bool m_isRenderTargetEnabled;
    ResourceHandle<RenderTargetResource> m_renderTarget;

private:
    glm::mat4 m_initialTransformLocal;
    glm::mat4 m_currentTransformWorld;
    bool m_shouldSyncWorldTransformToLocal;
};
