#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../../../Renderer/Components/EnvironmentProbeComponent.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class EnvironmentProbeComponentEdit : public BaseComponentEdit {
public:
    explicit EnvironmentProbeComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<EnvironmentProbeComponent>(entityId);
    }

    std::string getName() override {
        return "Environment probe";
    }

    void process(Identity::Type entityId) override {
        auto *probe = m_EditorUISystem->getComponent<EnvironmentProbeComponent>(entityId);
        if (probe == nullptr) {
            return;
        }

        ImGui::SeparatorText("Environment probe");

        ImGui::ColorEdit3("Debug color##PROBE_COLOR", (float *) &probe->m_DebugColor);
    }
};
