#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/PhysicsFixedJointComponent.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"

class PhysicsFixedJointComponentEdit : public BaseComponentEdit {
public:

    std::string getName() override {
        return "Physics fixed joint";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *body = entity.getComponent<PhysicsFixedJointComponent>();
        if (body == nullptr) {
            return;
        }

        EntityLinkedComponentEdit::process<PhysicsBodyComponent>(
            *system.m_EventManager,
            *system.m_EntityContext,
            body,
            "Attachment entity##TRANSFORM_PARENT_ENTITY_NAME"
        );
    }
};
