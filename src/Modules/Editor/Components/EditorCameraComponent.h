#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/BaseCameraComponent.h"

class TransformComponent;

class EditorCameraComponent final : public Component, public BaseCameraComponent {
public:
    enum OrthographicViewType {
        VIEW_FRONT,
        VIEW_BACK,
        VIEW_LEFT,
        VIEW_RIGHT,
        VIEW_TOP,
        VIEW_BOTTOM,
        // VIEW_ISOMETRIC   //45 DEGREE
    };

    inline static std::map<OrthographicViewType, std::string> OrthographicViewNames = {
        {VIEW_FRONT, "Front"},
        {VIEW_BACK, "Back"},
        {VIEW_LEFT, "Left"},
        {VIEW_RIGHT, "Right"},
        {VIEW_TOP, "Top"},
        {VIEW_BOTTOM, "Bottom"},
    };

private:
    TYPE_DEFINITION(EditorCameraComponent);
    inline static const std::string POSITION_KEY = "position";
    inline static const std::string VIEW_KEY = "viewDirection";
    inline static const std::string UP_KEY = "upDirection";
    inline static const std::string REL_ROTATION_KEY = "disableRelRotation";
    inline static const std::string ACTIVE_KEY = "isActive";
    inline static const std::string TYPE_KEY = "type";
    inline static const std::string SCALE_KEY = "orthoScale";

    inline static std::map<OrthographicViewType, std::pair<glm::vec3, glm::vec3> > OrthographicViews = {
        {VIEW_FRONT, {{0, 0, -1}, {0, 1, 0}}},
        {VIEW_BACK, {{0, 0, 1}, {0, 1, 0}}},
        {VIEW_LEFT, {{1, 0, 0}, {0, 1, 0}}},
        {VIEW_RIGHT, {{-1, 0, 0}, {0, 1, 0}}},
        {VIEW_TOP, {{0, -1, 0}, {0, 0, -1}}},
        {VIEW_BOTTOM, {{0, 1, 0}, {0, 0, 1}}},
    };

public:
    enum Type {
        TYPE_PERSPECTIVE,
        TYPE_ORTHOGRAPHIC
    };

    EditorCameraComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void rotateView(glm::vec2 viewChangeAlongScreenAxis);

    void moveView(glm::vec3 direction);

    void setOrthographic(bool);

    void setOrthographicViewType(OrthographicViewType);

    void setOrthographicViewTypeFocused(OrthographicViewType, TransformComponent &transform);

    void setOrthographicScale(float);

    bool isOrthographic() const {
        return m_Camera.isOrthographic;
    }

    Type m_type;
    float m_orthographicScale;
};
