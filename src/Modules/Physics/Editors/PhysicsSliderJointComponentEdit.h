#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Components/PhysicsSliderJointComponent.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"

class PhysicsSliderJointComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Physics slider joint";
    }

    void process(Entity &entity, EditorUISystem &system) override {
        auto *body = entity.getComponent<PhysicsSliderJointComponent>();
        if (body == nullptr) {
            return;
        }

        EntityLinkedComponentEdit::process<PhysicsBodyComponent>(
            *system.m_EventManager,
            *system.m_EntityContext,
            body,
            "Attachment entity##TRANSFORM_PARENT_ENTITY_NAME"
        );

        ImGui::InputFloat3("Axis", (float *) &body->m_axis);

        ImGui::Checkbox("Limits", &body->m_isLimitsEnabled);

        if (body->m_isLimitsEnabled) {
            ImGui::InputFloat2("Min/Max", (float *) &body->m_limits);
        }

        ImGui::Checkbox("Motor", &body->m_isMotorSettingsEnabled);

        if (body->m_isMotorSettingsEnabled) {
            ImGui::DragFloat("Force limit", &body->m_motorForceLimit, 0.1f);
            ImGui::DragFloat("Damping", &body->m_motorDamping, 0.1f);
            ImGui::DragFloat("Frequency", &body->m_motorFrequency, 0.1f);
        }
    }
};
