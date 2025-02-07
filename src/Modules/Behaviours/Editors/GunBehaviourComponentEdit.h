#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/GunBehaviourComponent.h"

class GunBehaviourComponentEdit : public BaseComponentEdit {
public:
    explicit GunBehaviourComponentEdit(EditorUISystem &editorSystem) : BaseComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Gun behaviour";
    }

    void process(Entity &entity, EditorUISystem &system) override {
        auto *component = entity.getComponent<GunBehaviourComponent>();
        if (component == nullptr) {
            return;
        }

        ImGui::Checkbox("Is active##IS_GUN_ACTIVE", &component->m_isActive);
    }
};
