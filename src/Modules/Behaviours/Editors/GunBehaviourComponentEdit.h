#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/GunBehaviourComponent.h"

inline void processGunBehaviourComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<GunBehaviourComponent *>(c);

    ImGui::Checkbox("Is active##IS_GUN_ACTIVE", &component->m_isActive);
}
