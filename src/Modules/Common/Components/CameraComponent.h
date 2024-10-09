#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../../Core/Helper/ComponentTransformEdit.h"

class CameraComponent : public Component, public ComponentTransformEdit {
TYPE_DEFINITION(CameraComponent);

public:
    enum Type {
        TYPE_FP,
        TYPE_FREE,
    };

    struct TBN {
        glm::vec3 view;
        glm::vec3 up;
        glm::vec3 right;
    };

    inline static std::map<Type, std::string> m_TypeNameMap = {
            {Type::TYPE_FP,   "FP"},
            {Type::TYPE_FREE, "FREE"},
    };

    CameraComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void registerWithSystems(EntityContext &ctx) override;

    void rotateView(glm::vec2 viewChangeAlongScreenAxis);

    glm::mat4 getEditorTransform() override;

    void setFromEditorTransform(const glm::mat4 &) override;

    Camera m_Camera;
    bool m_isActive;
    Type m_type;

    TBN calculateTBN(glm::vec3 viewDirection);

private:

    Type getTypeFromName(const std::string &name);
};
