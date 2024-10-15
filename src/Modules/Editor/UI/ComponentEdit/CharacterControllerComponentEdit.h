#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../../../Common/Components/CameraComponent.h"
#include "../../../Physics/Components/PhysicsCharacterComponent.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class CharacterControllerComponentEdit : public BaseComponentEdit {
public:
    explicit CharacterControllerComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<PhysicsCharacterComponent>(entityId);
    }

    std::string getName() override {
        return "Character controller";
    }

    void process(Identity::Type entityId) override {
        auto *comp = m_EditorUISystem->getComponent<PhysicsCharacterComponent>(entityId);
        if (comp == nullptr) {
            return;
        }

        if (ImGui::InputFloat("Height##CTT-HEIGHT", &comp->m_height, 0.1f, 0.2f, "%.2f")) {
        }
        
        if (ImGui::InputFloat("Radius##CTT-RADIUS", &comp->m_radius, 0.1f, 0.2f, "%.2f")) {
        }
    }
};
