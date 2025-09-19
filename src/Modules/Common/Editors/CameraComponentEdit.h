#pragma once

#include "../Components/CameraComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../Editor/Helpers/TransformHelper.h"

inline void processCameraComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<CameraComponent *>(c);

    if (component == nullptr) {
        return;
    }

    if (ImGui::Checkbox("Active##CAMERA_ACTIVE", &component->m_isActive)) {
        system.m_EventManager->queueEvent<CameraActivationEvent>(e->m_Id.id());
    }

    ImGui::Checkbox("Is relative rotation disabled##CAMERA_REL_ROTATION",
                    &component->m_isRelativeRotationDisabled);

    float fov = component->m_Camera.fieldOfView;
    if (ImGui::InputFloat("FOV##CAMERA_FOV", &fov, 1.0f, 5.0f, "%.0f")) {
        component->m_Camera.setFieldOfView(fov);
    }
}
