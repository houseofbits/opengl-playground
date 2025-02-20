#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Components/PhysicsTriggerShapeComponent.h"

class PhysicsTriggerShapeComponentEdit : public ComponentEdit<PhysicsTriggerShapeComponent> {
public:
    explicit PhysicsTriggerShapeComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Physics trigger shape";
    }

    void processEditor() override {
        if (m_component == nullptr) {
            return;
        }

        ImGui::Text("Box shape physics sensor");
    }
};
