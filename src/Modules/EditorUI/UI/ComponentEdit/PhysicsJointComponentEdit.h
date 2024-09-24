#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class PhysicsJointComponentEdit : public BaseComponentEdit {
public:
    explicit PhysicsJointComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<PhysicsJointComponent>(entityId);
    }

    std::string getName() override {
        return "Physics joint";
    }

    void process(Identity::Type entityId) override {
        auto *body = m_EditorUISystem->getComponent<PhysicsJointComponent>(entityId);
        if (body == nullptr) {
            return;
        }

        if (ImGui::BeginCombo("Target##TARGET_ENTITY_NAME", body->m_targetEntityName.c_str())) {
            for (const auto &entity: m_EditorUISystem->m_EntityContext->getAllEntities()) {
                auto* c = entity->getComponent<PhysicsBodyComponent>();
                if(c != nullptr) {
                    if (ImGui::Selectable(entity->m_Name.c_str(), entity->m_Name == body->m_targetEntityName)) {
                        body->m_targetEntityName = entity->m_Name;
                    }
                }
            }
            ImGui::EndCombo();
        }

        ImGui::InputFloat3("Attachment A", (float*)&body->m_localAttachmentA);
        ImGui::InputFloat3("Axis A", (float*)&body->m_axisA);
        ImGui::InputFloat3("Attachment B", (float*)&body->m_localAttachmentB);
        ImGui::InputFloat3("Axis B", (float*)&body->m_axisB);

        ImGui::Checkbox("Angular limits", &body->m_areLimitsEnabled);
        ImGui::Checkbox("Driven", &body->m_areDriveEnabled);

        if (body->m_areLimitsEnabled) {
            if(ImGui::InputFloat2("Angular limits", (float*)&angularLimits)) {
                body->m_angularLimits.x = glm::radians(angularLimits.x);
                body->m_angularLimits.y = glm::radians(angularLimits.y);
            } else {
                angularLimits.x = glm::degrees(body->m_angularLimits.x);
                angularLimits.y = glm::degrees(body->m_angularLimits.y);
            }
        }
        if (ImGui::Button("Rebuild joint")) {
            body->release();
        }
    }

    glm::vec2 angularLimits;
};
