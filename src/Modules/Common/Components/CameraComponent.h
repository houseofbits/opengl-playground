#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../../Core/Helper/ComponentTransformEdit.h"
#include "BaseCameraComponent.h"

class CameraComponent : public Component, public ComponentTransformEdit, public BaseCameraComponent {
TYPE_DEFINITION(CameraComponent);

public:
    enum Type {
        TYPE_FP,
        TYPE_FREE,
    };

    inline static std::map<Type, std::string> m_TypeNameMap = {
            {Type::TYPE_FP,   "FP"},
            {Type::TYPE_FREE, "FREE"},
    };

    CameraComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    /**
     * @deprecated
     */
    void registerWithSystems(EntityContext &ctx) override;

    void rotateView(glm::vec2 viewChangeAlongScreenAxis);

    void moveView(glm::vec3 direction);

    glm::mat4 getEditorTransform() override;

    void setFromEditorTransform(const glm::mat4 &) override;

    void updateTransformFromParent(const glm::mat4 &parent);

    void updateTransformWorld();

    Type m_type;

private:

    static Type getTypeFromName(const std::string &name);

    glm::mat4 m_initialTransformLocal;
    glm::mat4 m_currentTransformWorld;
    bool m_shouldSyncWorldTransformToLocal;
};
