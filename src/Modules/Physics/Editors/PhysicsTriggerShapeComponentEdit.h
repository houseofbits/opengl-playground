#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../Components/PhysicsTriggerShapeComponent.h"

inline void processPhysicsTriggerShapeComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<PhysicsTriggerShapeComponent *>(c);

    ImGui::Text("Box shape physics sensor");
}
