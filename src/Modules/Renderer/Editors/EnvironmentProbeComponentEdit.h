#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class EnvironmentProbeComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Environment probe";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *probe = entity.getComponent<EnvironmentProbeComponent>();
        if (probe == nullptr) {
            return;
        }

        ImGui::ColorEdit3("Debug color##PROBE_COLOR", (float *) &probe->m_DebugColor);
    }
};
