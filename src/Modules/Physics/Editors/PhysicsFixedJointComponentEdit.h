#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"
#include "../Components/PhysicsFixedJointComponent.h"

class PhysicsFixedJointComponentEdit : public BaseComponentEdit {
public:

    std::string getName() override {
        return "Physics fixed joint";
    }

    void process(Entity &entity, EditorUISystem& system) {
        auto *body = entity.getComponent<PhysicsFixedJointComponent>();
        if (body == nullptr) {
            return;
        }

        if (TargetEntityHelper<PhysicsBodyComponent>(system.m_EntityContext, body->m_targetEntityName,
                                                 "Target##TARGET_ENTITY_NAME")) {
            body->release();
        }
    }
};
