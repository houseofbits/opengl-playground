#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Components/PhysicsSliderJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"

class PhysicsSliderJointComponentEdit final : public ComponentEdit<PhysicsSliderJointComponent> {
public:
    explicit PhysicsSliderJointComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Physics slider joint";
    }

    void processEditor() override {
        if (m_component == nullptr) {
            return;
        }

        EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
            *m_editorSystem->m_EntityContext,
            m_component->m_targetEntityAName,
            "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
            "Self"
        );

        EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *m_editorSystem->m_EntityContext,
        m_component->m_targetEntityBName,
            "Attachment entity B##TRANSFORM_PARENT_ENTITY_NAME_B"
        );

        ImGui::InputFloat3("Axis", reinterpret_cast<float *>(&m_component->m_axis));

        ImGui::Checkbox("Limits", &m_component->m_isLimitsEnabled);

        if (m_component->m_isLimitsEnabled) {
            ImGui::InputFloat2("Min/Max", reinterpret_cast<float *>(&m_component->m_limits));
        }

        ImGui::Checkbox("Motor", &m_component->m_isMotorSettingsEnabled);

        if (m_component->m_isMotorSettingsEnabled) {
            ImGui::DragFloat("Force limit", &m_component->m_motorForceLimit, 0.1f);
            ImGui::DragFloat("Damping", &m_component->m_motorDamping, 0.1f);
            ImGui::DragFloat("Frequency", &m_component->m_motorFrequency, 0.1f);
        }
    }
};
