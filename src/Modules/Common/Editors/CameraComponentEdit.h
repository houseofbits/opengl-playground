#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/CameraComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Events/CameraActivationEvent.h"

class CameraComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Camera";
    }

    void process(Entity &entity, EditorUISystem &system) override {
        auto *camera = entity.getComponent<CameraComponent>();
        if (camera == nullptr) {
            return;
        }

        if (ImGui::Checkbox("Active##CAMERA_ACTIVE", &camera->m_isActive)) {
            system.m_EventManager->triggerEvent<CameraActivationEvent>(entity.m_Id.id());
        }

        ImGui::Checkbox("Is relative rotation disabled##CAMERA_REL_ROTATION", &camera->m_isRelativeRotationDisabled);

        float fov = camera->m_Camera.fieldOfView;
        if (ImGui::InputFloat("FOV##CAMERA_FOV", &fov, 1.0f, 5.0f, "%.0f")) {
            camera->m_Camera.setFieldOfView(fov);
        }
    }
};
