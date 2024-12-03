#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/CameraComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class CameraComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Camera";
    }

    void process(Entity &entity, EditorUISystem& system) {
        auto *camera = entity.getComponent<CameraComponent>();
        if (camera == nullptr) {
            return;
        }

        float fov = camera->m_Camera.fieldOfView;
        if (ImGui::InputFloat("FOV##CAMERA_FOV", &fov, 1.0f, 5.0f, "%.0f")) {
            camera->m_Camera.setFieldOfView(fov);
        }
    }
};
