#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Components/PhysicsSliderJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"

// class PhysicsSliderJointComponentEdit final : public ComponentEdit<PhysicsSliderJointComponent> {
// public:
//     explicit PhysicsSliderJointComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
//     }
//
//     std::string getName() override {
//         return "Physics slider joint";
//     }
//
//     void processEditor() override;
//
//     int getNumTransformTargets() override {
//         return 3;
//     }
//
//     void handleEntitySelection(Entity &e, Component *c) override;
//
//     TransformOption getTransformTargetOptions(int index) override;
//
//     glm::mat4 getWorldSpaceTransform(int index) override;
//
//     void setWorldSpaceTransform(glm::mat4 m, int index) const override;
//
// private:
//     [[nodiscard]] TransformComponent* getTransformComponentByName(const std::string& name) const;
//
//     TransformComponent *m_linkedTransformA{nullptr};
//     TransformComponent *m_linkedTransformB{nullptr};
// };


#include "PhysicsSliderJointComponentEdit.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../../Core/Helper/Math.h"

inline void processPhysicsSliderJointComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<PhysicsSliderJointComponent *>(c);

    EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *system.m_EntityContext,
        component->m_targetEntityAName,
        "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
        "Self"
    );

    EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
    *system.m_EntityContext,
    component->m_targetEntityBName,
        "Attachment entity B##TRANSFORM_PARENT_ENTITY_NAME_B"
    );

    // ImGui::InputFloat3("Axis", reinterpret_cast<float *>(&component->m_axis));
    if (ImGui::TreeNode("Attachment transform A")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixA, true, true, false);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Attachment transform B")) {
        TransformHelper::editTransform(component->m_localAttachmentMatrixB, true, true, false);
        ImGui::TreePop();
    }

    ImGui::Checkbox("Limits", &component->m_isLimitsEnabled);

    if (component->m_isLimitsEnabled) {
        ImGui::InputFloat2("Min/Max", reinterpret_cast<float *>(&component->m_limits));
    }

    ImGui::Checkbox("Motor", &component->m_isMotorSettingsEnabled);

    if (component->m_isMotorSettingsEnabled) {
        ImGui::DragFloat("Force limit", &component->m_motorForceLimit, 0.1f);
        ImGui::DragFloat("Damping", &component->m_motorDamping, 0.1f);
        ImGui::DragFloat("Frequency", &component->m_motorFrequency, 0.1f);
    }
}