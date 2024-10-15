#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"
#include "../../../Physics/Components/PhysicsSliderJointComponent.h"

class PhysicsSliderJointComponentEdit : public BaseComponentEdit {
public:
    explicit PhysicsSliderJointComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<PhysicsSliderJointComponent>(entityId);
    }

    std::string getName() override {
        return "Physics slider joint";
    }

    void process(Identity::Type entityId) override {
        auto *body = m_EditorUISystem->getComponent<PhysicsSliderJointComponent>(entityId);
        if (body == nullptr) {
            return;
        }

        if (TargetEntityHelper<PhysicsBodyComponent>(m_EditorUISystem->m_EntityContext, body->m_targetEntityName,
                                                 "Target##TARGET_ENTITY_NAME")) {
            body->release();
        }

        ImGui::InputFloat3("Axis", (float *) &body->m_axis);

        ImGui::Checkbox("Limits", &body->m_isLimitsEnabled);

        if (body->m_isLimitsEnabled) {
            ImGui::InputFloat2("Min/Max", (float *) &body->m_limits);
        }
    }
};
