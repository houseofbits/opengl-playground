#pragma once

#include <glm/mat4x4.hpp>
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui

class TransformHelper {
public:
    TransformHelper();

    static bool editTransform(glm::mat4 &m, const bool allowTranslation, const bool allowRotation,
                              const bool allowScale) {
        bool hasChanged = false;
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];

        ImGuizmo::DecomposeMatrixToComponents(&m[0][0], matrixTranslation, matrixRotation,
                                              matrixScale);

        if (allowTranslation) {
            if (ImGui::InputFloat3("Translation", matrixTranslation)) {
                ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &m[0][0]);
                hasChanged = true;
            }
        }
        if (allowRotation) {
            if (ImGui::InputFloat3("Rotation", matrixRotation)) {
                ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &m[0][0]);
                hasChanged = true;
            }
        }
        if (allowScale) {
            if (ImGui::InputFloat3("Scaling", matrixScale)) {
                ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &m[0][0]);
                hasChanged = true;
            }
        }

        return hasChanged;
    }
};
