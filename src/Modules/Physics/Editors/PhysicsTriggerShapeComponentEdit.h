#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Components/PhysicsTriggerShapeComponent.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"

class PhysicsTriggerShapeComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Physics trigger shape";
    }

    void process(Entity &entity, EditorUISystem &system) override {
        auto *body = entity.getComponent<PhysicsTriggerShapeComponent>();
        if (body == nullptr) {
            return;
        }

        ImGui::Text("Box shape physics sensor");
    }
};
