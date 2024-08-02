#pragma once

#include "../../../Common/Components/TransformComponent.h"
#include "BaseComponentEdit.h"

class TransformComponentEdit : public BaseComponentEdit {
public:
    explicit TransformComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<TransformComponent>(entityId);
    }

    std::string getName() override {
        return "Transform";
    }

    void process(Identity::Type entityId) override {
        auto *transform = m_EditorUISystem->getComponent<TransformComponent>(entityId);
        if (transform == nullptr) {
            return;
        }

        ImGui::SeparatorText("Transform");

        float matrixTranslation[3], matrixRotation[3], matrixScale[3];

        ImGuizmo::DecomposeMatrixToComponents(&transform->m_ModelMatrix[0][0], matrixTranslation, matrixRotation, matrixScale);
        if (transform->m_isTranslationEnabled) {
            ImGui::InputFloat3("Translation", matrixTranslation);
        }
        if (transform->m_isRotationEnabled) {
            ImGui::InputFloat3("Rotation", matrixRotation);
        }
        if (transform->m_isScalingEnabled) {
            ImGui::InputFloat3("Scaling", matrixScale);
        }
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &transform->m_ModelMatrix[0][0]);
    }
};
