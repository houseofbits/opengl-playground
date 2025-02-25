#pragma once

#include "../Components/TransformComponent.h"
#include "../../Editor/UI/ComponentEdit.h"

class TransformComponentTransformEdit final : public ComponentEdit<TransformComponent> {
public:
    explicit TransformComponentTransformEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    int getNumTransformTargets() override {
        return 1;
    }

    TransformOption getTransformTargetOptions(int index) override;

    glm::mat4 getWorldSpaceTransform(int index) override {
        assert(m_component != nullptr);

        return m_component->getWorldTransform();
    }

    void setWorldSpaceTransform(glm::mat4 m, int index) const override {
        assert(m_component != nullptr);

        m_component->setWorldTransform(m);
    }
};
