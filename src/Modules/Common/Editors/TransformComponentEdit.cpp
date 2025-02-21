#include "../../../Core/API.h"
#include "./TransformComponentEdit.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../../Editor/Helpers/TransformHelper.h"

void TransformComponentEdit::processEditor() {
    if (m_component == nullptr) {
        return;
    }

    if (glm::mat4 m = m_component->m_transform;
        TransformHelper::editTransform(m,
                                       m_component->m_isTranslationEnabled,
                                       m_component->m_isRotationEnabled,
                                       m_component->m_isScalingEnabled
        )) {
        m_component->setWorldTransform(m);
    }

    ImGui::Checkbox("Is relative rotation disabled##TRANSFORM_REL_ROTATION",
                    &m_component->m_isRelativeRotationDisabled);

    // EntityLinkedComponentEdit::process<TransformComponent>(
    //     *system.m_EventManager,
    //     *system.m_EntityContext,
    //     transform,
    //     "Parent entity##TRANSFORM_PARENT_ENTITY_NAME"
    // );
}

TransformComponentEdit::TransformOption TransformComponentEdit::getTransformTargetOptions(int index) {
    return {
        "Primary transform",
        true,
        true,
        true,
        ImGuizmo::TRANSLATE,
        false,
        true,
    };
}
