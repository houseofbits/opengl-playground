#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Components/PhysicsSliderJointComponent.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"

class PhysicsSliderJointComponentEdit final : public ComponentEdit<PhysicsSliderJointComponent> {
public:
    explicit PhysicsSliderJointComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Physics slider joint";
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

    TransformComponent *m_linkedTransformA{nullptr};
    TransformComponent *m_linkedTransformB{nullptr};
};
