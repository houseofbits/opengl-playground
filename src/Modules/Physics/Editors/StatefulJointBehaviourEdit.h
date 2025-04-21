#pragma once
#include "../Helpers/Behaviours/StatefulJointBehaviour.h"
#include "../../../SourceLibs/imgui/imgui.h"

inline void StatefulJointBehaviourEditor(StatefulJointBehaviour *b, EditorUISystem &system) {
    ImGui::Checkbox("Is initially open", &b->m_isInitiallyOpen);
    ImGui::Checkbox("Is fixed on final state", &b->m_isFixedOnFinalState);
    ImGui::InputFloat("Velocity", &b->m_velocity);
}
