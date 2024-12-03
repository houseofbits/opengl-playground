#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class PhysicsHingeJointComponentEdit : public BaseComponentEdit {
public:

    std::string getName() override {
        return "Physics hinge joint";
    }

    void process(Entity &entity, EditorUISystem& system) {
        auto *body = entity.getComponent<PhysicsHingeJointComponent>();
        if (body == nullptr) {
            return;
        }

        if (TargetEntityHelper<PhysicsBodyComponent>(system.m_EntityContext, body->m_targetEntityName,
                                                 "Target##TARGET_ENTITY_NAME")) {
            body->release();
        }

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
