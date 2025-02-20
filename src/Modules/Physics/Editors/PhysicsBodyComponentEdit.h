#pragma once

#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"

class PhysicsBodyComponent;

class PhysicsBodyComponentEdit : public ComponentEdit<PhysicsBodyComponent> {
public:
    explicit PhysicsBodyComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Physics body";
    }

    void processEditor() override;

    std::string m_meshPath;
};
