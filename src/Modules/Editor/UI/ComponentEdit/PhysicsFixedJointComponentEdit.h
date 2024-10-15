#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"
#include "../../../Physics/Components/PhysicsFixedJointComponent.h"

class PhysicsFixedJointComponentEdit : public BaseComponentEdit {
public:
    explicit PhysicsFixedJointComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<PhysicsFixedJointComponent>(entityId);
    }

    std::string getName() override {
        return "Physics fixed joint";
    }

    void process(Identity::Type entityId) override {
        auto *body = m_EditorUISystem->getComponent<PhysicsFixedJointComponent>(entityId);
        if (body == nullptr) {
            return;
        }

        if (TargetEntityHelper<PhysicsBodyComponent>(m_EditorUISystem->m_EntityContext, body->m_targetEntityName,
                                                 "Target##TARGET_ENTITY_NAME")) {
            body->release();
        }
    }
};
