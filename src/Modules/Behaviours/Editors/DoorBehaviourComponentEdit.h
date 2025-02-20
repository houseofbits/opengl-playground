#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/DoorBehaviourComponent.h"

class DoorBehaviourComponentEdit : public BaseComponentEdit {
public:
    explicit DoorBehaviourComponentEdit(EditorUISystem &editorSystem) : BaseComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Door behaviour";
    }

    void processEditor() override;

    void handleEntitySelection(Entity &e, Component *c) override;

private:
    DoorBehaviourComponent *m_component{nullptr};
};
