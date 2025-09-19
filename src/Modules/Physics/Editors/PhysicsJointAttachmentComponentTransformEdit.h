#pragma once

#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Components/PhysicsJointAttachmentComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../../Core/Helper/Math.h"

class PhysicsCharacterComponent;

class PhysicsJointAttachmentComponentTransformEdit final : public ComponentEdit<PhysicsJointAttachmentComponent> {
public:
    explicit PhysicsJointAttachmentComponentTransformEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
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
    PhysicsCharacterComponent *m_parentCharacter{nullptr};
};
