#pragma once

#include "../../Editor/UI/ComponentEdit.h"
#include "../Components/PhysicsShapeComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"

class PhysicsShapeComponentTransformEdit final : public ComponentEdit<PhysicsShapeComponent>
{
public:
    explicit PhysicsShapeComponentTransformEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    int getNumTransformTargets() override {
        return 1;
    }

    void handleEntitySelection(Entity &e, Component *c) override;

    TransformOption getTransformTargetOptions(int index) override;

    glm::mat4 getWorldSpaceTransform(int index) override;

    void setWorldSpaceTransform(glm::mat4 m, int index) const override;

private:
    TransformComponent *m_parentTransform{nullptr};
};
