#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/PhysicsCharacterComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/UI/ComponentEdit.h"

class CharacterControllerComponentEdit : public ComponentEdit<PhysicsCharacterComponent> {
public:
    explicit CharacterControllerComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Character controller";
    }

    void processEditor() override {
        if (m_component == nullptr) {
            return;
        }

        if (ImGui::InputFloat("Height##CTT-HEIGHT", &m_component->m_height, 0.1f, 0.2f, "%.2f")) {
        }

        if (ImGui::InputFloat("Radius##CTT-RADIUS", &m_component->m_radius, 0.1f, 0.2f, "%.2f")) {
        }
    }
};
