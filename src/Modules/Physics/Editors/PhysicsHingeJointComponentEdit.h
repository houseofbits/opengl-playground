#pragma once

#include "../../Common/Components/TransformComponent.h"
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
        return 3;
    }

    void handleEntitySelection(Entity &e, Component *c) override;

    TransformOption getTransformTargetOptions(int index) override;

    glm::mat4 getWorldSpaceTransform(int index) override;

    void setWorldSpaceTransform(glm::mat4 m, int index) const override;

private:
    [[nodiscard]] TransformComponent* getTransformComponentByName(const std::string& name) const;

    glm::vec2 angularLimits{0, 0};
    TransformComponent *m_linkedTransformA{nullptr};
    TransformComponent *m_linkedTransformB{nullptr};
};
