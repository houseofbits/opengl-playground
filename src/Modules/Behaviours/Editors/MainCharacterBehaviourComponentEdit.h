#pragma once

#include "../Components/MainCharacterBehaviourComponent.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"

inline void processMainCharacterBehaviourComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<MainCharacterBehaviourComponent *>(c);

    if (ImGui::Checkbox("Active##MAIN_CHARACTER_ACTIVE", &component->m_isActive)) {
    }

    ImGui::InputFloat("Mouse look speed", &component->m_mouseLookSpeed);
    ImGui::InputFloat("Movement speed", &component->m_movementSpeed);

    if (EntityLinkedComponentEdit::processBasic<CameraComponent>(
        *system.m_EntityContext,
        component->m_cameraName,
        "Attached camera##CHARACTER_LINKED_CAMERA_NAME",
        "Self"
    )) {
        if (auto cameraEntity = system.m_EntityContext->findEntity(component->m_cameraName)) {
            component->setCameraEntity(*cameraEntity);
        } else {
            component->clearCameraEntity();
        }
    }

    ImGui::InputFloat3("Camera attachment", &component->m_cameraAttachmentPosition[0]);
    ImGui::InputFloat3("Initial look direction", &component->m_lookingDirection[0]);
}
