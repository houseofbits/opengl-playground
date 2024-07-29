#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h" //Note: Order dependent include. Should be after ImGui
#include "../../../SourceLibs/imgui/ImGuiFileDialog.h"

#include "../Systems/EditorUISystem.h"
#include "MainToolbarUI.h"


MainToolbarUI::MainToolbarUI(EditorUISystem *editor) : m_EditorUISystem(editor),
                                                       m_isEditWindowVisible(true),
                                                       m_renderShaderType(0),
                                                       m_currentGizmoOperation(ImGuizmo::TRANSLATE),
                                                       m_currentGizmoMode(ImGuizmo::WORLD) {
}

void MainToolbarUI::process() {

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) {
                sendSaveEvent();
            }
            if (ImGui::MenuItem("Edit components", nullptr, m_isEditWindowVisible)) {
                m_isEditWindowVisible = !m_isEditWindowVisible;
                sendEditorStateEvent();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("View shaded", nullptr, m_renderShaderType == 0)) {
                sendUIEvent(EditorUIEvent::TOGGLE_RENDER_SHADED);
                m_renderShaderType = 0;
            }
            if (ImGui::MenuItem("View probes", nullptr, m_renderShaderType == 1)) {
                sendUIEvent(EditorUIEvent::TOGGLE_RENDER_PROBES);
                m_renderShaderType = 1;
            }
            if (ImGui::MenuItem("View reflections", nullptr, m_renderShaderType == 2)) {
                sendUIEvent(EditorUIEvent::TOGGLE_RENDER_REFLECTIONS);
                m_renderShaderType = 2;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Update probes")) {
                sendUIEvent(EditorUIEvent::TRIGGER_PROBE_RENDER);
            }
            ImGui::EndMenu();
        }

        auto *transform = m_EditorUISystem->getSelectedTransformComponent();
        if (transform != nullptr) {
            ImGui::Spacing();
            processTransformTypeDropdown(transform);
            processTransformSpaceDropdown();
        }

        ImGui::EndMainMenuBar();
    }
}

void MainToolbarUI::processTransformSpaceDropdown() {
    static const char *selected_transform_space_label = "WORLD";
    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##TRANSFORM_SPACE", selected_transform_space_label)) {
        if (ImGui::Selectable("WORLD", m_currentGizmoMode == ImGuizmo::WORLD)) {
            m_currentGizmoMode = ImGuizmo::WORLD;
            selected_transform_space_label = "WORLD";
        }

        if (ImGui::Selectable("LOCAL", m_currentGizmoMode == ImGuizmo::LOCAL)) {
            m_currentGizmoMode = ImGuizmo::LOCAL;
            selected_transform_space_label = "LOCAL";
        }

        ImGui::EndCombo();
    }
}

void MainToolbarUI::processTransformTypeDropdown(TransformComponent *transform) {
    static const char *selected_transform_type_label = "Translate";

    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##TRANSFORM_TYPE", selected_transform_type_label)) {
        if (transform->m_isTranslationEnabled) {
            if (ImGui::Selectable("Translate", m_currentGizmoOperation == ImGuizmo::TRANSLATE)) {
                m_currentGizmoOperation = ImGuizmo::TRANSLATE;
                selected_transform_type_label = "Translate";
            }
        }
        if (transform->m_isRotationEnabled) {
            if (ImGui::Selectable("Rotate", m_currentGizmoOperation == ImGuizmo::ROTATE)) {
                m_currentGizmoOperation = ImGuizmo::ROTATE;
                selected_transform_type_label = "Rotate";
            }
        }
        if (transform->m_isScalingEnabled) {
            if (ImGui::Selectable("Scale", m_currentGizmoOperation == ImGuizmo::SCALE)) {
                m_currentGizmoOperation = ImGuizmo::SCALE;
                selected_transform_type_label = "Scale";
            }
        }
//        if (transform->m_isScalingEnabled) {
//            if (ImGui::Selectable("Universal", m_currentGizmoOperation == ImGuizmo::SCALEU)) {
//                m_currentGizmoOperation = ImGuizmo::SCALEU;
//                selected_transform_type_label = "Universal";
//            }
//        }
        ImGui::EndCombo();
    }
}

void MainToolbarUI::sendSaveEvent() {
    auto evt = new EditorUIEvent();
    evt->m_Type = EditorUIEvent::SAVE;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}

void MainToolbarUI::sendEditorStateEvent() {
    auto evt = new EditorUIEvent();
    evt->m_Type = m_isEditWindowVisible ? EditorUIEvent::EDITOR_ENABLED : EditorUIEvent::EDITOR_DISABLED;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}

void MainToolbarUI::sendUIEvent(EditorUIEvent::Type type) {
    auto evt = new EditorUIEvent();
    evt->m_Type = type;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}
