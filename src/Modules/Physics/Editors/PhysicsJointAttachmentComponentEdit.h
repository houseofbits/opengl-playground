#pragma once

#include "StatefulJointBehaviourEdit.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Editor/Helpers/TargetEntityHelper.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../../Editor/UI/BaseComponentTransformEdit.h"
#include "../Components/PhysicsJointAttachmentComponent.h"
#include "../../Editor/Helpers/TransformHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processPhysicsJointAttachmentComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    const auto component = dynamic_cast<PhysicsJointAttachmentComponent *>(c);

    TransformHelper::editTransform(component->m_localAttachmentMatrix, true, true, false);
}
