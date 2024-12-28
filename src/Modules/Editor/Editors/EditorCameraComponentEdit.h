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

    void process(Entity &entity, EditorUISystem &system) override {
        auto *camera = entity.getComponent<EditorCameraComponent>();
        if (camera == nullptr) {
            return;
        }

        if (ImGui::Checkbox("Active##CAMERA_ACTIVE", &camera->m_isActive)) {
            system.m_EventManager->queueEvent<CameraActivationEvent>(entity.m_Id.id());
        }

        static bool isOrthographic = camera->m_type == EditorCameraComponent::TYPE_ORTHOGRAPHIC;
        if (ImGui::Checkbox("Orthographic##CAMERA_ORTHOGRAPHIC", &isOrthographic)) {
            camera->setOrthographic(isOrthographic);
        }

        if (ImGui::DragFloat("Orthographic scale##CAMERA_ORTHOGRAPHIC_SCALE", &camera->m_orthographicScale, 0.5f)) {
            camera->setOrthographicScale(camera->m_orthographicScale);
        }
    }
};
