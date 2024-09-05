#pragma once

#include "../../../Common/Components/TransformComponent.h"
#include "BaseComponentEdit.h"

class DoorComponentEdit : public BaseComponentEdit {
public:
    explicit DoorComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<DoorComponent>(entityId);
    }

    std::string getName() override {
        return "Door";
    }

    void process(Identity::Type entityId) override {
        auto *door = m_EditorUISystem->getComponent<DoorComponent>(entityId);
        if (door == nullptr) {
            return;
        }

        ImGui::SeparatorText("Door");

        ImGui::InputFloat3("Axis", (float*)&door->m_axis);
        ImGui::InputFloat3("Pivot", (float*)&door->m_rotationPivot);

        if (ImGui::Button("Activate")) {

        }
    }
};
