#include "./CameraComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui

void CameraComponentEdit::processEditor() {
    if (m_component == nullptr) {
        return;
    }

    if (ImGui::Checkbox("Active##CAMERA_ACTIVE", &m_component->m_isActive)) {
        m_editorSystem->m_EventManager->queueEvent<CameraActivationEvent>(m_entity->m_Id.id());
    }

    ImGui::Checkbox("Is relative rotation disabled##CAMERA_REL_ROTATION",
                    &m_component->m_isRelativeRotationDisabled);

    float fov = m_component->m_Camera.fieldOfView;
    if (ImGui::InputFloat("FOV##CAMERA_FOV", &fov, 1.0f, 5.0f, "%.0f")) {
        m_component->m_Camera.setFieldOfView(fov);
    }
}

CameraComponentEdit::TransformOption CameraComponentEdit::getTransformTargetOptions(int index) {
    return {
        "Camera transform",
        true,
        true,
        false,
        ImGuizmo::TRANSLATE,
        false,
    };
}
