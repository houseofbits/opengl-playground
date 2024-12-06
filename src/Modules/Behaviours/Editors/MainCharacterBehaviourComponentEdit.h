#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/MainCharacterBehaviourComponent.h"

class MainCharacterBehaviourComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Main character behaviour";
    }

    void process(Entity &entity, EditorUISystem &system) override {
        auto *body = entity.getComponent<MainCharacterBehaviourComponent>();
        if (body == nullptr) {
            return;
        }


        if (ImGui::Checkbox("Active##MAIN_CHARACTER_ACTIVE", &body->m_isActive)) {

        }
    }

    std::string m_meshPath;
};
