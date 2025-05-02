#pragma once

#include "../../Common/Components/TransformComponent.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"

class PhysicsFixedJointComponent;

class PhysicsFixedJointComponentTransformEdit final : public ComponentEdit<PhysicsFixedJointComponent> {
public:
    explicit PhysicsFixedJointComponentTransformEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    int getNumTransformTargets() override {
        return 3;
    }

    void handleEntitySelection(Entity &e, Component *c) override;

    TransformOption getTransformTargetOptions(int index) override;

    glm::mat4 getWorldSpaceTransform(int index) override;

    void setWorldSpaceTransform(glm::mat4 m, int index) const override;

private:
    [[nodiscard]] TransformComponent *getTransformComponentByName(const std::string &name) const;

    TransformComponent *m_linkedTransformA{nullptr};
    TransformComponent *m_linkedTransformB{nullptr};
};
