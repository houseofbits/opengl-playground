#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/CameraComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../Events/CameraActivationEvent.h"

class CameraComponentEdit final : public ComponentEdit<CameraComponent> {
public:
    explicit CameraComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Camera";
    }

    int getNumTransformTargets() override {
        return 1;
    }

    TransformOption getTransformTargetOptions(int index) override;

    void processEditor() override;

    glm::mat4 getWorldSpaceTransform(int index) override {
        assert(m_component != nullptr);

        return m_component->getWorldTransform();
    }

    void setWorldSpaceTransform(glm::mat4 m, int index) const override {
        assert(m_component != nullptr);

        m_component->setWorldTransform(m);
    }
};
