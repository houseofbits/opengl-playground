#pragma once

#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../Components/PhysicsFixedJointComponent.h"


class PhysicsFixedJointComponentEdit : public ComponentEdit<PhysicsFixedJointComponent> {
public:
    explicit PhysicsFixedJointComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Physics fixed joint";
    }

    void processEditor() override;
};
