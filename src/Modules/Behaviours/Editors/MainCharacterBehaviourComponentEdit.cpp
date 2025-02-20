#include "MainCharacterBehaviourComponentEdit.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui

void MainCharacterBehaviourComponentEdit::processEditor() {
    if (m_component == nullptr) {
        return;
    }

    if (ImGui::Checkbox("Active##MAIN_CHARACTER_ACTIVE", &m_component->m_isActive)) {

    }
}
