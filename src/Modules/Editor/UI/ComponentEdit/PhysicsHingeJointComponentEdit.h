#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "../TargetEntityHelper.h"
#include "BaseComponentEdit.h"

class PhysicsHingeJointComponentEdit : public BaseComponentEdit {
public:
    explicit PhysicsHingeJointComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem), angularLimits(.0) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<PhysicsHingeJointComponent>(entityId);
    }

    std::string getName() override {
        return "Physics hinge joint";
    }

    void process(Identity::Type entityId) override {
        auto *body = m_EditorUISystem->getComponent<PhysicsHingeJointComponent>(entityId);
        if (body == nullptr) {
            return;
        }

        TargetEntityHelper<PhysicsBodyComponent>(m_EditorUISystem->m_EntityContext, body->m_targetEntityName,
                                                 "Target##TARGET_ENTITY_NAME");

        ImGui::InputFloat3("Attachment A", (float *) &body->m_localAttachmentA);
        ImGui::InputFloat3("Axis A", (float *) &body->m_axisA);
        ImGui::InputFloat3("Attachment B", (float *) &body->m_localAttachmentB);
        ImGui::InputFloat3("Axis B", (float *) &body->m_axisB);

        ImGui::Checkbox("Angular limits", &body->m_isLimitsEnabled);
        ImGui::Checkbox("Lock on limits", &body->m_isLockingToLimitsEnabled);

        if (body->m_isLimitsEnabled) {
            if (ImGui::InputFloat2("Angular limits", (float *) &angularLimits)) {
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
