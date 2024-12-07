#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/DoorBehaviourComponent.h"

class DoorBehaviourComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Door behaviour";
    }

    void process(Entity &entity, EditorUISystem &system) override {
        auto *body = entity.getComponent<DoorBehaviourComponent>();
        if (body == nullptr) {
            return;
        }

    }

    std::string m_meshPath;
};
