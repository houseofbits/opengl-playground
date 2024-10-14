#include "MainToolbarUI.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../../Physics/Components/CharacterControllerComponent.h"
#include "../Systems/EditorUISystem.h"


MainToolbarUI::MainToolbarUI(EditorUISystem *editor) : m_EditorUISystem(editor),
                                                       m_isEditWindowVisible(true),
                                                       m_isSimulationEnabled(true),
                                                       m_renderShaderType(0),
                                                       m_previousSelectedCameraComponentId(0),
                                                       m_selectedCameraComponentId(0) {
}

void MainToolbarUI::process() {

    if (ImGui::BeginMainMenuBar()) {
        ImGui::PushID(0);
        if (!m_isSimulationEnabled) {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(0, 200, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(0, 170, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0, 150, 0));
            if (ImGui::Button("Run")) {
                runSimulation();
            }
            ImGui::PopStyleColor(3);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(200, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(170, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(150, 0, 0));
            if (ImGui::Button("Stop")) {
                stopSimulation();
            }
            ImGui::PopStyleColor(3);
        }
        ImGui::PopID();

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Save")) {
                sendSaveEvent();
            }
            if (ImGui::MenuItem("Edit components", nullptr, m_isEditWindowVisible)) {
                m_isEditWindowVisible = !m_isEditWindowVisible;
                sendEditorStateEvent();
            }
            if (ImGui::MenuItem("Simulation enabled", nullptr, m_isSimulationEnabled)) {
                m_isSimulationEnabled = !m_isSimulationEnabled;
                sendUIEvent(m_isSimulationEnabled ? EditorUIEvent::TOGGLE_SIMULATION_ENABLED : EditorUIEvent::TOGGLE_SIMULATION_DISABLED);
            }
            if (ImGui::MenuItem("Reset rigid bodies")) {
                m_isSimulationEnabled = false;
                sendUIEvent(EditorUIEvent::RESET_TO_INITIAL_TRANSFORM);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Update probes")) {
                sendUIEvent(EditorUIEvent::TRIGGER_PROBE_RENDER);
            }
            ImGui::EndMenu();
        }

        processViewMenu();

        ImGui::Spacing();

        if (!m_isSimulationEnabled) {
            m_EditorUISystem->m_transformGizmo.processToolbar(*m_EditorUISystem->m_EntityContext);
        }

        ImGui::EndMainMenuBar();
    }
}

void MainToolbarUI::sendSaveEvent() {
    m_EditorUISystem->m_EventManager->queueEvent<EditorUIEvent>(EditorUIEvent::SAVE);
}

void MainToolbarUI::sendEditorStateEvent() {
    m_EditorUISystem->m_EventManager->queueEvent<EditorUIEvent>(
            m_isEditWindowVisible ? EditorUIEvent::EDITOR_ENABLED : EditorUIEvent::EDITOR_DISABLED);
}

void MainToolbarUI::sendUIEvent(EditorUIEvent::Type type) {
    m_EditorUISystem->m_EventManager->queueEvent<EditorUIEvent>(type);
}

void MainToolbarUI::processViewMenu() {
    for (const auto comp: m_EditorUISystem->getComponentContainer<CameraComponent>()) {
        if (comp.second->m_isActive) {
            m_selectedCameraComponentId = (int) comp.first;
        }
    }

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
        ImGui::SeparatorText("Cameras");

        for (const auto comp: m_EditorUISystem->getComponentContainer<CameraComponent>()) {
            Entity *e = m_EditorUISystem->m_EntityContext->getEntity(comp.first);
            if (ImGui::MenuItem(e->getListName().c_str(), nullptr, comp.second->m_isActive)) {
                m_selectedCameraComponentId = (int) comp.first;
            }
        }

        ImGui::EndMenu();
    }

    activateCameras();
}

void MainToolbarUI::runSimulation() {
    m_isSimulationEnabled = true;
    sendUIEvent(EditorUIEvent::TOGGLE_SIMULATION_ENABLED);
    m_isEditWindowVisible = false;
    sendEditorStateEvent();

    m_previousSelectedCameraComponentId = m_selectedCameraComponentId;
    auto cc = m_EditorUISystem->getComponentContainer<CharacterControllerComponent>();
    if (!cc.empty()) {
        auto* ccCam = m_EditorUISystem->getComponent<CameraComponent>(cc.begin()->first);
        if (ccCam!= nullptr) {
            m_selectedCameraComponentId = (int) ccCam->m_EntityId.id();
        }
    }

    activateCameras();
}

void MainToolbarUI::stopSimulation() {
    m_isSimulationEnabled = false;
    sendUIEvent(EditorUIEvent::TOGGLE_SIMULATION_DISABLED);
    sendUIEvent(EditorUIEvent::RESET_TO_INITIAL_TRANSFORM);
    m_isEditWindowVisible = true;
    sendEditorStateEvent();

    if (m_previousSelectedCameraComponentId) {
        m_selectedCameraComponentId = m_previousSelectedCameraComponentId;
        m_previousSelectedCameraComponentId = 0;
        activateCameras();
    }
}

void MainToolbarUI::activateCameras() {
    for (const auto comp: m_EditorUISystem->getComponentContainer<CameraComponent>()) {
        comp.second->m_isActive = m_selectedCameraComponentId == comp.first;
    }
}
