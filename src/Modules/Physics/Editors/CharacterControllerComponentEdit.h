#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/PhysicsCharacterComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/UI/ComponentEdit.h"

inline void processPhysicsCharacterControllerComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<PhysicsCharacterComponent *>(c);

    if (ImGui::InputFloat("Height##CTT-HEIGHT", &component->m_height, 0.1f, 0.2f, "%.2f")) {
    }

    if (ImGui::InputFloat("Radius##CTT-RADIUS", &component->m_radius, 0.1f, 0.2f, "%.2f")) {
    }
};
