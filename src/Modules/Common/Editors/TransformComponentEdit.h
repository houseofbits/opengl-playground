#pragma once

#include "EntityLinkedComponentEdit.h"
#include "../Components/TransformComponent.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class TransformComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Transform";
    }

    void process(Entity &entity, EditorUISystem &system) override {
        auto *transform = entity.getComponent<TransformComponent>();
        if (transform == nullptr) {
            return;
        }

        float matrixTranslation[3], matrixRotation[3], matrixScale[3];

        ImGuizmo::DecomposeMatrixToComponents(&transform->m_transform[0][0], matrixTranslation, matrixRotation,
                                              matrixScale);
        if (transform->m_isTranslationEnabled) {
            ImGui::InputFloat3("Translation", matrixTranslation);
        }
        if (transform->m_isRotationEnabled) {
            ImGui::InputFloat3("Rotation", matrixRotation);
        }
        if (transform->m_isScalingEnabled) {
            ImGui::InputFloat3("Scaling", matrixScale);
        }

        ImGui::Checkbox("Is relative rotation disabled##TRANSFORM_REL_ROTATION", &transform->m_isRelativeRotationDisabled);

        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale,
                                                &transform->m_transform[0][0]);

        EntityLinkedComponentEdit::process<TransformComponent>(
            *system.m_EventManager,
            *system.m_EntityContext,
            transform,
            "Parent entity##TRANSFORM_PARENT_ENTITY_NAME"
        );
    }
};
