#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/EditorCameraComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../../Common/Events/CameraActivationEvent.h"

class EditorCameraComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Editor camera";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *camera = entity.getComponent<EditorCameraComponent>();
        if (camera == nullptr) {
            return;
        }

        if (ImGui::Checkbox("Active##CAMERA_ACTIVE", &camera->m_isActive)) {
            system.m_EventManager->triggerEvent<CameraActivationEvent>(entity.m_Id.id());
        }
    }
};
