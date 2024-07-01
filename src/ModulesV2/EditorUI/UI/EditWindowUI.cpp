#include "EditWindowUI.h"

#include <utility>
#include "../../../CoreV2/Events/EntityCreationEvent.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../Systems/EditorUISystem.h"

EditWindowUI::EditWindowUI(EditorUISystem *editor) : m_selectedEntity(-1),
                                                     m_EditorUISystem(editor),
                                                     m_lightProjectorPath(),
                                                     m_meshModelPath() {

}

void EditWindowUI::process() {
    ImGui::SetNextWindowPos(ImVec2(0, 25));
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoCollapse)) {

        if (ImGui::Button("Create light")) {
            sendEntityCreationEvent("Light");
        }

        if (m_selectedEntity >= 0) {
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                sendEntityRemovalEvent();
                m_selectedEntity = -1;
            }
        }

        processEntitiesList();

        Entity *e = m_EditorUISystem->m_EntityContext->getEntity(m_selectedEntity);
        if (e != nullptr) {
            ImGui::InputText("Name", &e->m_Name);

            processEditLightComponent();
            processEditMeshComponent();
            processEditCameraComponent();
            processEditTransformComponent();
        }
    }
    ImGui::End();
}

void EditWindowUI::processEntitiesList() {
    ImGui::BeginListBox("##ENTITY_LIST", ImVec2(ImGui::GetWindowWidth() - 15, 200));
    for (const auto &light: m_EditorUISystem->m_lightComponents) {
        Entity *e = m_EditorUISystem->m_EntityContext->getEntity(light.first);
        if (e != nullptr) {
            std::string name = e->m_Name;
            if (light.second->m_Type == LightComponent::SPOT) {
                name.append(" [SPOT LIGHT]");
            } else if (light.second->m_Type == LightComponent::OMNI) {
                name.append(" [OMNI LIGHT]");
            } else if (light.second->m_Type == LightComponent::DIRECT) {
                name.append(" [DIRECT LIGHT]");
            }

            if (ImGui::Selectable(name.c_str(), m_selectedEntity == light.first)) {
                m_selectedEntity = (int) light.first;
                m_lightProjectorPath = light.second->m_Projection().m_Path;
            }
        }
    }
    for (const auto &mesh: m_EditorUISystem->m_meshComponents) {
        Entity *e = m_EditorUISystem->m_EntityContext->getEntity(mesh.first);
        if (e != nullptr) {
            std::string name = e->m_Name;
            name.append(" [MESH]");

            if (ImGui::Selectable(name.c_str(), m_selectedEntity == mesh.first)) {
                m_selectedEntity = (int) mesh.first;
                m_meshModelPath = mesh.second->m_Mesh().m_Path;
            }
        }
    }
    for (const auto &camera: m_EditorUISystem->m_cameraComponents) {
        Entity *e = m_EditorUISystem->m_EntityContext->getEntity(camera.first);
        if (e != nullptr) {
            std::string name = e->m_Name;
            name.append(" [CAMERA]");

            if (ImGui::Selectable(name.c_str(), m_selectedEntity == camera.first)) {
                m_selectedEntity = (int) camera.first;
            }
        }
    }
    ImGui::EndListBox();
}

void EditWindowUI::processEditLightComponent() {
    if (m_EditorUISystem->m_lightComponents.find(m_selectedEntity) == m_EditorUISystem->m_lightComponents.end()) {
        return;
    }

    LightComponent *light = m_EditorUISystem->m_lightComponents[m_selectedEntity];

    ImGui::SeparatorText("Light");
    ImGui::ColorEdit3("Color##LIGHT_COLOR", (float *) &light->m_Color);
    ImGui::InputFloat("Intensity##LIGHT_INTENSITY", &light->m_Intensity, 0.1f, 1.0f, "%.1f");
    ImGui::InputFloat("Attenuation##LIGHT_ATTENUATION", &light->m_Attenuation, 0.1f, 1.0f, "%.1f");
    ImGui::InputFloat("Beam angle##LIGHT_BEAM_ANGLE", &light->m_beamAngle, 0.5f, 1.0f, "%.0f");

    if (ImGui::InputText("Projector", &m_lightProjectorPath)) {
        m_EditorUISystem->m_ResourceManager->request(light->m_Projection, m_lightProjectorPath);
    }
}

void EditWindowUI::processEditMeshComponent() {
    if (m_EditorUISystem->m_meshComponents.find(m_selectedEntity) == m_EditorUISystem->m_meshComponents.end()) {
        return;
    }

    StaticMeshComponent *mesh = m_EditorUISystem->m_meshComponents[m_selectedEntity];

    ImGui::SeparatorText("Mesh");

    if (ImGui::InputText("Model", &m_meshModelPath)) {
        m_EditorUISystem->m_ResourceManager->request(mesh->m_Mesh, m_meshModelPath);
    }
}

void EditWindowUI::processEditTransformComponent() {
    if (m_EditorUISystem->m_transformComponents.find(m_selectedEntity) == m_EditorUISystem->m_transformComponents.end()) {
        return;
    }

    ImGui::SeparatorText("Transform");
    TransformComponent *transform = m_EditorUISystem->m_transformComponents[m_selectedEntity];

    float matrixTranslation[3], matrixRotation[3], matrixScale[3];

    ImGuizmo::DecomposeMatrixToComponents(&transform->m_ModelMatrix[0][0], matrixTranslation, matrixRotation, matrixScale);
    if (transform->m_isTranslationEnabled) {
        ImGui::InputFloat3("Translation", matrixTranslation);
    }
    if (transform->m_isRotationEnabled) {
        ImGui::InputFloat3("Rotation", matrixRotation);
    }
    if (transform->m_isScalingEnabled) {
        ImGui::InputFloat3("Scaling", matrixScale);
    }
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &transform->m_ModelMatrix[0][0]);
}

void EditWindowUI::processEditCameraComponent() {
    if (m_EditorUISystem->m_cameraComponents.find(m_selectedEntity) == m_EditorUISystem->m_cameraComponents.end()) {
        return;
    }

    ImGui::SeparatorText("Camera");
    CameraComponent *camera = m_EditorUISystem->m_cameraComponents[m_selectedEntity];

    float fov = camera->m_Camera.fieldOfView;
    if (ImGui::InputFloat("FOV##CAMERA_FOV", &fov, 1.0f, 5.0f, "%.0f")) {
        camera->m_Camera.setFieldOfView(fov);
    }
}

bool EditWindowUI::isTransformComponentSelected() {
    if (m_selectedEntity < 0
        || m_EditorUISystem->m_transformComponents.find(m_selectedEntity) == m_EditorUISystem->m_transformComponents.end()) {
        return false;
    }

    return true;
}

void EditWindowUI::sendEntityCreationEvent(std::string name) {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::CREATE;
    evt->m_Name = std::move(name);
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}

void EditWindowUI::sendEntityRemovalEvent() {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::REMOVE;
    evt->m_entityId = m_selectedEntity;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}
