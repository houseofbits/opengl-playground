#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Components/PhysicsSliderJointComponent.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"

class PhysicsSliderJointComponentEdit : public BaseComponentEdit {
public:

    std::string getName() override {
        return "Physics slider joint";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *body = entity.getComponent<PhysicsSliderJointComponent>();
        if (body == nullptr) {
            return;
        }

        if (TargetEntityHelper<PhysicsBodyComponent>(system.m_EntityContext, body->m_targetEntityName,
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
