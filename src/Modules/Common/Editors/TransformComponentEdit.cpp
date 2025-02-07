#include "../../../Core/API.h"
#include "./TransformComponentEdit.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui

void TransformComponentEdit::processEditor(Entity &entity, Component *component) {
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

    // EntityLinkedComponentEdit::process<TransformComponent>(
    //     *system.m_EventManager,
    //     *system.m_EntityContext,
    //     transform,
    //     "Parent entity##TRANSFORM_PARENT_ENTITY_NAME"
    // );
}

void TransformComponentEdit::updateTransform(TransformComponent *comp, float t[3], float r[3], float s[3]) {
    glm::mat4 matrix = glm::mat4(1.0f);
    ImGuizmo::RecomposeMatrixFromComponents(t, r, s, &matrix[0][0]);

    comp->setWorldTransform(matrix);
}

TransformComponentEdit::TransformOption TransformComponentEdit::getTransformTargetOptions(int index) {
    return {
        "Primary transform",
        true,
        true,
        true,
        ImGuizmo::TRANSLATE,
    };
}

