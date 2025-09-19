#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../../Editor/UI/ComponentEdit.h"

inline void processEnvironmentProbeComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<EnvironmentProbeComponent *>(c);

    ImGui::ColorEdit3("Debug color##PROBE_COLOR", reinterpret_cast<float *>(&component->m_DebugColor));
};
