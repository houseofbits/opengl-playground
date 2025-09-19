#include "./CameraComponentTransformEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui

CameraComponentTransformEdit::TransformOption CameraComponentTransformEdit::getTransformTargetOptions(int index) {
    return {
        "Camera transform",
        true,
        true,
        false,
        ImGuizmo::TRANSLATE,
        false,
    };
}
