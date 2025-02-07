#pragma once

// #include "EntityLinkedComponentEdit.h"
#include "../Components/TransformComponent.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class TransformComponentEdit : public BaseComponentEdit {
public:
    explicit TransformComponentEdit(EditorUISystem &editorSystem) : BaseComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Transform";
    }

    void processEditor(Entity &e, Component *c) override;

    int getNumTransformTargets() override {
        return 1;
    }

    TransformOption getTransformTargetOptions(int index) override;

    glm::mat4 getWorldSpaceTransform(int index) override {
        return m_transformComponent->getWorldTransform();
    }

    void setWorldSpaceTransform(glm::mat4 m, int index) const override {
        m_transformComponent->setWorldTransform(m);
    }

    static void updateTransform(TransformComponent *comp, float t[3], float r[3], float s[3]);

private:
    TransformComponent *m_transformComponent{nullptr};
};
