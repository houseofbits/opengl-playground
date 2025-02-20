#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"
#include "../../Editor/UI/ComponentEdit.h"

class PhysicsHingeJointComponent;

class PhysicsHingeJointComponentEdit : public ComponentEdit<PhysicsHingeJointComponent> {
public:
    explicit PhysicsHingeJointComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Physics hinge joint";
    }

    void processEditor() override;

    int getNumTransformTargets() override {
        return 1;
    }

    TransformOption getTransformTargetOptions(int index) override;

    glm::mat4 getWorldSpaceTransform(int index) override;

    void setWorldSpaceTransform(glm::mat4 m, int index) const override;

    glm::vec2 angularLimits{0, 0};
};
