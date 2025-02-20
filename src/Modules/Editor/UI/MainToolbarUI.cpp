#include "MainToolbarUI.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../Systems/EditorUISystem.h"
#include "../Components/EditorCameraComponent.h"
#include "../../Common/Events/CameraActivationEvent.h"

MainToolbarUI::MainToolbarUI(EditorUISystem *editor) : m_EditorUISystem(editor),
                                                       m_renderShaderType(0) {
}

void MainToolbarUI::process() {
    if (ImGui::BeginMainMenuBar()) {
        ImGui::PushID(0);
        if (m_EditorUISystem->m_isEditorModeEnabled) {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(0, 200, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(0, 170, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0, 150, 0));
            if (ImGui::Button("Run")) {
                enableGameMode();
            }
            ImGui::PopStyleColor(3);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(200, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(170, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(150, 0, 0));
            if (ImGui::Button("Stop")) {
                enableEditorMode();
            }
            ImGui::PopStyleColor(3);
        }
        ImGui::PopID();

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Save")) {
                m_EditorUISystem->m_EventManager->queueEvent<EditorUIEvent>(EditorUIEvent::SAVE);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Update probes")) {
                sendUIEvent(EditorUIEvent::TRIGGER_PROBE_RENDER);
            }
            ImGui::EndMenu();
        }

        processViewMenu();

        ImGui::Spacing();

        if (m_EditorUISystem->m_isEditorModeEnabled) {
            // m_EditorUISystem->m_transformGizmo.processToolbar(*m_EditorUISystem->m_EntityContext);
            m_EditorUISystem->m_transformHelper.processToolbar();
        }

        ImGui::EndMainMenuBar();
    }
}

void MainToolbarUI::sendUIEvent(EditorUIEvent::Type type) const {
    m_EditorUISystem->m_EventManager->queueEvent<EditorUIEvent>(type);
}

void MainToolbarUI::processViewMenu() {
    if (ImGui::BeginMenu("View")) {
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
        if (ImGui::MenuItem("View physics", nullptr, m_renderShaderType == 3)) {
            sendUIEvent(EditorUIEvent::TOGGLE_RENDER_PHYSICS);
            m_renderShaderType = 3;
        }
        ImGui::SeparatorText("Cameras");

        for (const auto &[id, camera]: m_EditorUISystem->m_editorCameraComponentRegistry->container()) {
            Entity *e = m_EditorUISystem->m_EntityContext->getEntity(id);
            if (ImGui::MenuItem(e->getListName().c_str(), nullptr, camera->m_isActive)) {
                m_EditorUISystem->m_EventManager->queueEvent<CameraActivationEvent>(id);
            }
        }

        for (const auto &[id, camera]: m_EditorUISystem->m_cameraComponentRegistry->container()) {
            Entity *e = m_EditorUISystem->m_EntityContext->getEntity(id);
            if (ImGui::MenuItem(e->getListName().c_str(), nullptr, camera->m_isActive)) {
                m_EditorUISystem->m_EventManager->queueEvent<CameraActivationEvent>(id);
            }
        }

        ImGui::SeparatorText("Views");

        for (const auto &[id, camera]: m_EditorUISystem->m_editorCameraComponentRegistry->container()) {
            if (camera->m_isActive && camera->isOrthographic()) {
                for (const auto &[type, name]: EditorCameraComponent::OrthographicViewNames) {
                    if (ImGui::MenuItem(name.c_str())) {
                        camera->setOrthographicViewType(type);

                        // if (m_EditorUISystem->m_selectedEntityId > 0) {
                        //     if (auto selectedTransformComponent = m_EditorUISystem->m_EntityContext->getEntityComponent<
                        //         TransformComponent>(m_EditorUISystem->m_selectedEntityId)) {
                        //         camera->setOrthographicViewTypeFocused(type, *selectedTransformComponent);
                        //     }
                        // }
                    }
                }

                break;
            }
        }

        ImGui::EndMenu();
    }
}

void MainToolbarUI::enableGameMode() const {
    m_EditorUISystem->m_EventManager->queueEvent<SystemEvent>(SystemEvent::REQUEST_GAME_MODE);
}

void MainToolbarUI::enableEditorMode() const {
    m_EditorUISystem->m_EventManager->queueEvent<SystemEvent>(SystemEvent::REQUEST_EDITOR_MODE);
}
