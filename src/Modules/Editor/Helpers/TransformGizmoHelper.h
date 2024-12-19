#pragma once

#include "../../../Core/Reflection/Identity.h"
#include "../../../Core/Entity/EntityContext.h"
#include "../../../Core/Helper/ComponentTransformEdit.h"
#include "../../../Renderer/Camera/Camera.h"

class TransformGizmoHelper {
public:
    TransformGizmoHelper();

    void processGizmo(EntityContext &ctx, Identity::Type &selectedEntityId, Camera &camera);

    void processToolbar(EntityContext &ctx);

private:
    const static std::map<long, std::string> GizmoOperationNames;
    const static std::map<long, std::string> GizmoModeNames;

    enum TransformOptionType {
        TRANSFORM_PRIMARY = 0,
        TRANSFORM_HINGE_JOINT,
        TRANSFORM_CAMERA,
    };

    struct TransformOption {
        std::string name;
        bool isTranslationEnabled;
        bool isRotationEnabled;
        bool isScalingEnabled;
        long defaultOperation;
    };

    void handleEntitySelection(EntityContext &ctx, Identity::Type id);

    void handleOptionSelection(EntityContext &ctx, TransformOptionType optionType);

    void populateTransformOptions(Entity &entity);

    void getTransformFromSelectedComponent(EntityContext &ctx);

    void setSelectedComponentTransform(EntityContext &ctx);

    template<class T>
    void setTransformToEditorComponent(EntityContext &ctx) const {
        auto edit = static_cast<ComponentTransformEdit *>(ctx.getEntityComponent<T>(m_selectedEntityId));
        if (edit != nullptr) {
            edit->setWorldTransform(m_transform);
        }
    }

    template<class T>
    void getTransformFromEditorComponent(EntityContext &ctx) {
        auto edit = static_cast<ComponentTransformEdit *>(ctx.getEntityComponent<T>(m_selectedEntityId));
        if (edit != nullptr) {
            m_transform = edit->getWorldTransform();
        }
    }

    Identity::Type m_selectedEntityId;
    std::map<TransformOptionType, TransformOption> m_transformOptions;
    TransformOptionType m_selectedTransformOption;
    long m_selectedGizmoOperation;
    long m_selectedGizmoMode;
    bool m_isBoundsTransformAllowed;
    glm::mat4 m_transform;
};
