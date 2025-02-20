#pragma once

#include "../../Editor/UI/ComponentEdit.h"
#include "../Components/MainCharacterBehaviourComponent.h"

class MainCharacterBehaviourComponentEdit final : public ComponentEdit<MainCharacterBehaviourComponent> {
public:
    explicit MainCharacterBehaviourComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Main character behaviour";
    }

    void processEditor() override;
};
