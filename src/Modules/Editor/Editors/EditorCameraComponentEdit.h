#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/EditorCameraComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/UI/BaseComponentTransformEdit.h"
#include "../../Common/Events/CameraActivationEvent.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processEditorCameraComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<EditorCameraComponent *>(c);

    if (ImGui::Checkbox("Active##CAMERA_ACTIVE", &component->m_isActive)) {
        system.m_EventManager->queueEvent<CameraActivationEvent>(e->m_Id.id());
    }

    static bool isOrthographic = component->m_type == EditorCameraComponent::TYPE_ORTHOGRAPHIC;
    if (ImGui::Checkbox("Orthographic##CAMERA_ORTHOGRAPHIC", &isOrthographic)) {
        component->setOrthographic(isOrthographic);
    }

    if (ImGui::DragFloat("Orthographic scale##CAMERA_ORTHOGRAPHIC_SCALE", &component->m_orthographicScale, 0.5f)) {
        component->setOrthographicScale(component->m_orthographicScale);
    }
}
