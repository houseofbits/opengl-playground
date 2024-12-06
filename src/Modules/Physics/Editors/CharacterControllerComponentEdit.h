#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Components/PhysicsCharacterComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class CharacterControllerComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Character controller";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *comp = entity.getComponent<PhysicsCharacterComponent>();
        if (comp == nullptr) {
            return;
        }

        if (ImGui::InputFloat("Height##CTT-HEIGHT", &comp->m_height, 0.1f, 0.2f, "%.2f")) {
        }
        
        if (ImGui::InputFloat("Radius##CTT-RADIUS", &comp->m_radius, 0.1f, 0.2f, "%.2f")) {
        }
    }
};
