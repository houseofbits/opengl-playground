#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class RigidBodyComponentEdit : public BaseComponentEdit {
public:
    explicit RigidBodyComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<RigidBodyComponent>(entityId);
    }

    std::string getName() override {
        return "Rigid body";
    }

    void process(Identity::Type entityId) override {
        auto *body = m_EditorUISystem->getComponent<RigidBodyComponent>(entityId);
        if (body == nullptr) {
            return;
        }

        ImGui::SeparatorText("Rigid body");

        ImGui::InputFloat("Density", &body->m_density);
        ImGui::InputFloat("Dynamic friction", &body->m_friction.x);
        ImGui::InputFloat("Static friction", &body->m_friction.y);
        ImGui::InputFloat("Restitution", &body->m_restitution);
    }
};
