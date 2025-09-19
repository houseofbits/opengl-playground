#include "../../../Core/API.h"
#include "./TransformComponentTransformEdit.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui

TransformComponentTransformEdit::TransformOption TransformComponentTransformEdit::getTransformTargetOptions(int index) {
    return {
        "Primary transform",
        true,
        true,
        true,
        ImGuizmo::TRANSLATE,
        false,
        true,
    };
}
