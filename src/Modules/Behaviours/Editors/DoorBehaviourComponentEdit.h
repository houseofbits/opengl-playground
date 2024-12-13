#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/DoorBehaviourComponent.h"

class DoorBehaviourComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Door behaviour";
    }

    void process(Entity &entity, EditorUISystem &system) override {
        auto *body = entity.getComponent<DoorBehaviourComponent>();
        if (body == nullptr) {
            return;
        }

        ImGui::Checkbox("Is initially open", &body->m_isInitiallyOpen);
        ImGui::Checkbox("Is fixed on final state", &body->m_isFixedOnFinalState);

        ImGui::InputFloat("Velocity", &body->m_velocity);
    }

    std::string m_meshPath;
};
