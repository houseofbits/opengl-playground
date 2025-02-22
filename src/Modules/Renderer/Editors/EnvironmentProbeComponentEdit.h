#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/UI/ComponentEdit.h"

class EnvironmentProbeComponentEdit : public ComponentEdit<EnvironmentProbeComponent> {
public:
    explicit EnvironmentProbeComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Environment probe";
    }

    void processEditor() override {
        ImGui::ColorEdit3("Debug color##PROBE_COLOR", reinterpret_cast<float *>(&m_component->m_DebugColor));
    }
};
