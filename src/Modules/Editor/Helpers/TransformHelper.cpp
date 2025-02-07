#include "TransformHelper.h"
#include "../Systems/EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui

const std::map<long, std::string> GizmoOperationNames = {
    {ImGuizmo::TRANSLATE, "Translate"},
    {ImGuizmo::ROTATE, "Rotate"},
    {ImGuizmo::SCALE, "Scale"},
};
const std::map<long, std::string> GizmoModeNames = {
    {ImGuizmo::WORLD, "WORLD"},
    {ImGuizmo::LOCAL, "LOCAL"}
};

void TransformHelper::processToolbar() {
    auto *e = m_UISystem->m_EntityContext->getEntity(m_UISystem->m_selectedEntityId);
    if (!e) {
        return;
    }

    handleEntitySelection(*e);

    ImGui::SameLine();
    ImGui::PushItemWidth(150);

    processTransformTargetDropdown(*e);

    if (m_pSelectedComponentEdit != nullptr) {
        ImGui::SameLine();
        ImGui::PushItemWidth(100);

        processTransformOperationDropdown();

        ImGui::SameLine();
        ImGui::PushItemWidth(100);

        processTransformSpaceDropdown();
    }
}

void TransformHelper::processGizmo(Camera &camera) {
}

void TransformHelper::handleEntitySelection(Entity &e) {
    if (m_selectedEntityId != m_UISystem->m_selectedEntityId) {
        m_selectedEntityId = static_cast<int>(m_UISystem->m_selectedEntityId);
        m_selectedTransformTargetIndex = -1;
        m_pSelectedComponentEdit = nullptr;
        m_selectedTransformSpace = ImGuizmo::WORLD;
        m_selectedTransformOperation = 0;

        for (const auto &component: e.m_Components) {
            auto editor = m_UISystem->m_componentEditors[component->getTypeName()];
            if (editor && editor->getNumTransformTargets() > 0) {
                m_pSelectedComponentEdit = editor;
                m_selectedTransformTargetIndex = 0;

                auto options = editor->getTransformTargetOptions(0);
                if (options.isTranslationEnabled) {
                    m_selectedTransformOperation = ImGuizmo::TRANSLATE;
                } else if (options.isRotationEnabled) {
                    m_selectedTransformOperation = ImGuizmo::ROTATE;
                } else if (options.isScalingEnabled) {
                    m_selectedTransformOperation = ImGuizmo::SCALE;
                }

                break;
            }
        }
    }
}

void TransformHelper::processTransformTargetDropdown(Entity &e) {
    std::string selectedName = "";
    if (m_pSelectedComponentEdit != nullptr) {
        selectedName = m_pSelectedComponentEdit->getTransformTargetOptions(m_selectedTransformTargetIndex).name;
    }

    if (ImGui::BeginCombo("##TRANSFORM_TYPE", selectedName.c_str())) {
        for (const auto &component: e.m_Components) {
            auto editor = m_UISystem->m_componentEditors[component->getTypeName()];
            if (editor && editor->getNumTransformTargets() > 0) {
                for (int i = 0; i < editor->getNumTransformTargets(); ++i) {
                    bool isSelected = m_pSelectedComponentEdit == editor && i == m_selectedTransformTargetIndex;

                    if (ImGui::Selectable(editor->getTransformTargetOptions(i).name.c_str(), isSelected)) {
                        m_selectedTransformTargetIndex = i;
                        m_pSelectedComponentEdit = editor;
                    }
                }
            }
        }

        ImGui::EndCombo();
    }
}

void TransformHelper::processTransformOperationDropdown() {
    if (GizmoOperationNames.count(m_selectedTransformOperation) == 0) {
        return;
    }

    if (ImGui::BeginCombo("##TRANSFORM_OPERATION", GizmoOperationNames.at(m_selectedTransformOperation).c_str())) {
        auto option = m_pSelectedComponentEdit->getTransformTargetOptions(m_selectedTransformTargetIndex);

        if (option.isTranslationEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::TRANSLATE).c_str(),
                                  m_selectedTransformOperation == ImGuizmo::TRANSLATE)) {
                m_selectedTransformOperation = ImGuizmo::TRANSLATE;
            }
        }
        if (option.isRotationEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::ROTATE).c_str(),
                                  m_selectedTransformOperation == ImGuizmo::ROTATE)) {
                m_selectedTransformOperation = ImGuizmo::ROTATE;
            }
        }
        if (option.isScalingEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::SCALE).c_str(),
                                  m_selectedTransformOperation == ImGuizmo::SCALE)) {
                m_selectedTransformOperation = ImGuizmo::SCALE;
            }
        }

        ImGui::EndCombo();
    }
}

void TransformHelper::processTransformSpaceDropdown() {
    if (GizmoModeNames.count(m_selectedTransformSpace) == 0) {
        return;
    }

    if (ImGui::BeginCombo("##TRANSFORM_SPACE", GizmoModeNames.at(m_selectedTransformSpace).c_str())) {
        if (ImGui::Selectable(GizmoModeNames.at(ImGuizmo::WORLD).c_str(),
                              m_selectedTransformSpace == ImGuizmo::WORLD)) {
            m_selectedTransformSpace = ImGuizmo::WORLD;
        }

        if (ImGui::Selectable(GizmoModeNames.at(ImGuizmo::LOCAL).c_str(),
                              m_selectedTransformSpace == ImGuizmo::LOCAL)) {
            m_selectedTransformSpace = ImGuizmo::LOCAL;
        }

        ImGui::EndCombo();
    }
}
