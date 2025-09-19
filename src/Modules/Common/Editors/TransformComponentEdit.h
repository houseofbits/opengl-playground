#pragma once

#include "../Components/TransformComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../Editor/Helpers/TransformHelper.h"

inline void processTransformComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<TransformComponent *>(c);

    if (glm::mat4 m = component->getWorldTransform();
        TransformHelper::editTransform(m,
                                       component->m_isTranslationEnabled,
                                       component->m_isRotationEnabled,
                                       component->m_isScalingEnabled
        )) {
        component->setWorldTransform(m, true);
    }

    ImGui::Checkbox("Is relative rotation disabled##TRANSFORM_REL_ROTATION",
                    &component->m_isRelativeRotationDisabled);

    // EntityLinkedComponentEdit::process<TransformComponent>(
    //     *system.m_EventManager,
    //     *system.m_EntityContext,
    //     transform,
    //     "Parent entity##TRANSFORM_PARENT_ENTITY_NAME"
    // );
}
