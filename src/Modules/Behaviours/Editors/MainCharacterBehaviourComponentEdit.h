#pragma once

#include "../Components/MainCharacterBehaviourComponent.h"
#include "../../../SourceLibs/imgui/imgui.h"

inline void processMainCharacterBehaviourComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<MainCharacterBehaviourComponent *>(c);

    if (ImGui::Checkbox("Active##MAIN_CHARACTER_ACTIVE", &component->m_isActive)) {

    }
}
