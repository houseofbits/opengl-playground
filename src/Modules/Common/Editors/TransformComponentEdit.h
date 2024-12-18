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
            if (ImGui::InputFloat3("Translation", matrixTranslation)) {
                updateTransform(transform, matrixTranslation, matrixRotation, matrixScale);
            }
        }
        if (transform->m_isRotationEnabled) {
            if (ImGui::InputFloat3("Rotation", matrixRotation)) {
                updateTransform(transform, matrixTranslation, matrixRotation, matrixScale);
            }
        }
        if (transform->m_isScalingEnabled) {
            if (ImGui::InputFloat3("Scaling", matrixScale)) {
                updateTransform(transform, matrixTranslation, matrixRotation, matrixScale);
            }
        }

        ImGui::Checkbox("Is relative rotation disabled##TRANSFORM_REL_ROTATION", &transform->m_isRelativeRotationDisabled);

        EntityLinkedComponentEdit::process<TransformComponent>(
            *system.m_EventManager,
            *system.m_EntityContext,
            transform,
            "Parent entity##TRANSFORM_PARENT_ENTITY_NAME"
        );
    }

    static void updateTransform(TransformComponent* comp, float t[3], float r[3], float s[3]) {
        glm::mat4 matrix = glm::mat4(1.0f);
        ImGuizmo::RecomposeMatrixFromComponents(t, r, s, &matrix[0][0]);

        comp->setWorldTransform(matrix);
    }
};
