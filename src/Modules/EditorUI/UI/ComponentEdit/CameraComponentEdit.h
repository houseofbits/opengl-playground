#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../../../Renderer/Components/CameraComponent.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class CameraComponentEdit : public BaseComponentEdit {
public:
    explicit CameraComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<CameraComponent>(entityId);
    }

    std::string getName() override {
        return "Camera";
    }

    void process(Identity::Type entityId) override {
        auto *camera = m_EditorUISystem->getComponent<CameraComponent>(entityId);
        if (camera == nullptr) {
            return;
        }

        ImGui::SeparatorText("Camera");

        float fov = camera->m_Camera.fieldOfView;
        if (ImGui::InputFloat("FOV##CAMERA_FOV", &fov, 1.0f, 5.0f, "%.0f")) {
            camera->m_Camera.setFieldOfView(fov);
        }
    }
};
