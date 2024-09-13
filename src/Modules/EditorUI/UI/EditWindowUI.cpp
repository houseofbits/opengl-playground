#include "EditWindowUI.h"
#include "../../../Core/Events/EntityCreationEvent.h"
#include "../../../SourceLibs/imgui/ImGuiFileDialog.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../../Physics/Components/CharacterControllerComponent.h"
#include "../../Physics/Components/PhysicsBodyComponent.h"
#include "../../Physics/Components/PhysicsJointComponent.h"
#include "../Systems/EditorUISystem.h"
#include "ComponentEdit/CameraComponentEdit.h"
#include "ComponentEdit/CharacterControllerComponentEdit.h"
#include "ComponentEdit/EnvironmentProbeComponentEdit.h"
#include "ComponentEdit/LightComponentEdit.h"
#include "ComponentEdit/PhysicsBodyComponentEdit.h"
#include "ComponentEdit/PhysicsJointComponentEdit.h"
#include "ComponentEdit/StaticMeshComponentEdit.h"
#include "ComponentEdit/TransformComponentEdit.h"
#include "FileDialogHelper.h"
#include <utility>

std::vector<std::string> ENTITY_CREATION_OPTIONS = {
        "SpotLight",
        "OmniLight",
        "DirectLight",
        "StaticMesh",
        "EnvironmentProbe",
        "RigidBody",
        "CharacterController",
        "Door"};

std::vector<std::pair<std::string, std::string>> COMPONENT_NAMES = {
        {"light", "Light"},
        {"camera", "Camera"},
        {"mesh", "Render mesh"},
        {"probe", "Environment probe"},
        {"sky", "Skybox"},
        {"cct", "Character"},
        {"physicsBody", "Physics body"},
        {"physicsJoint", "Physics joint"}
};

EditWindowUI::EditWindowUI(EditorUISystem *editor) : m_selectedEntity(-1),
                                                     m_EditorUISystem(editor),
                                                     m_lightProjectorPath(),
                                                     m_isBoundsTransformAllowed(false),
                                                     m_meshModelPath(),
                                                     m_meshMaterialPath(),
                                                     m_selectedEntityCreationType(0),
                                                     m_selectedComponentCreationType(0),
                                                     m_componentEditors(),
                                                     m_entityListFilter() {

    m_componentEditors[TransformComponent::TypeName()] = new TransformComponentEdit(editor);
    m_componentEditors[StaticMeshComponent::TypeName()] = new StaticMeshComponentEdit(editor);
    m_componentEditors[LightComponent::TypeName()] = new LightComponentEdit(editor);
    m_componentEditors[CameraComponent::TypeName()] = new CameraComponentEdit(editor);
    m_componentEditors[EnvironmentProbeComponent::TypeName()] = new EnvironmentProbeComponentEdit(editor);
    m_componentEditors[PhysicsBodyComponent::TypeName()] = new PhysicsBodyComponentEdit(editor);
    m_componentEditors[CharacterControllerComponent::TypeName()] = new CharacterControllerComponentEdit(editor);
    m_componentEditors[PhysicsJointComponent::TypeName()] = new PhysicsJointComponentEdit(editor);
}

void EditWindowUI::process() {
    ImGui::SetNextWindowPos(ImVec2(0, 25));
    ImGui::SetNextWindowSize(ImVec2(320, 1000));
    if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoCollapse)) {

        ImGui::PushItemWidth(100);
        if (ImGui::BeginCombo("##ENTITY_TYPE", ENTITY_CREATION_OPTIONS[m_selectedEntityCreationType].c_str())) {
            for (int i = 0; i < ENTITY_CREATION_OPTIONS.size(); i++) {
                if (ImGui::Selectable(ENTITY_CREATION_OPTIONS[i].c_str(), m_selectedEntityCreationType == i)) {
                    m_selectedEntityCreationType = i;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("Create")) {
            sendEntityCreationEvent(ENTITY_CREATION_OPTIONS[m_selectedEntityCreationType], "NEW " + ENTITY_CREATION_OPTIONS[m_selectedEntityCreationType]);
        }

        if (m_selectedEntity >= 0) {
            ImGui::SameLine();
            if (ImGui::Button("Clone")) {
                sendEntityCloneEvent(m_selectedEntity);
            }

            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                sendEntityRemovalEvent();
                m_selectedEntity = -1;
            }
        }


        if (ImGui::InputText("##FILTER_STRING", &m_filterString)) {
        }

        if (ImGui::BeginCombo("##ENTITY_FILTER", "Filter")) {
            for (const auto &edit: m_componentEditors) {
                ImGui::Checkbox(edit.second->getName().c_str(), &m_entityListFilter[edit.first]);
            }
            m_selectedEntity = -1;
            ImGui::EndCombo();
        }

        processEntitiesList();

        Entity *e = m_EditorUISystem->m_EntityContext->getEntity(m_selectedEntity);
        if (e != nullptr) {
            ImGui::InputText("Name", &e->m_Name);

            for (const auto &edit: m_componentEditors) {
                if (edit.second->isEntityEditable(m_selectedEntity) && ImGui::CollapsingHeader(edit.second->getName().c_str())) {
                    edit.second->process(m_selectedEntity);
                }
            }

            ImGui::SeparatorText("Add component");

            ImGui::PushItemWidth(150);
            if (ImGui::BeginCombo("##COMPONENT_TYPE", COMPONENT_NAMES[m_selectedComponentCreationType].second.c_str())) {
                for (int i = 0; i < COMPONENT_NAMES.size(); i++) {
                    if (ImGui::Selectable(COMPONENT_NAMES[i].second.c_str(), m_selectedComponentCreationType == i)) {
                        m_selectedComponentCreationType = i;
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button("Add")) {
                sendComponentCreationEvent(COMPONENT_NAMES[m_selectedComponentCreationType].first);
            }

            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                sendComponentRemovalEvent(COMPONENT_NAMES[m_selectedComponentCreationType].first);
            }
        }

        ImGui::End();
    }
}

void EditWindowUI::processEntitiesList() {
    if (ImGui::BeginListBox("##ENTITY_LIST", ImVec2(ImGui::GetWindowWidth() - 15, 200))) {
        for (const auto &entity: m_EditorUISystem->m_EntityContext->getAllEntities()) {
            std::string name = entity->m_Name;

            bool isEditable = false;
            if (!m_filterString.empty()) {
                if (StringUtils::toLowerCase(entity->m_Name).find(StringUtils::toLowerCase(m_filterString)) != std::string::npos) {
                    isEditable = true;
                }
            }

            for (const auto &edit: m_componentEditors) {
                if (edit.second->isEntityEditable(entity->m_Id.id()) && m_entityListFilter[edit.first]) {
                    isEditable = true;
                    break;
                }
            }

            if (isEditable && ImGui::Selectable(name.c_str(), m_selectedEntity == entity->m_Id.id())) {
                m_selectedEntity = (int) entity->m_Id.id();
            }
        }

        ImGui::EndListBox();
    }
}

bool EditWindowUI::isTransformComponentSelected() {
    if (m_selectedEntity < 0 || m_EditorUISystem->getComponent<TransformComponent>(m_selectedEntity) == nullptr) {
        return false;
    }

    return true;
}

void EditWindowUI::sendEntityCreationEvent(std::string name, std::string entityName) {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::CREATE;
    evt->m_ConfigurationName = std::move(name);
    evt->m_name = std::move(entityName);
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}

void EditWindowUI::sendEntityRemovalEvent() {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::REMOVE;
    evt->m_entityId = m_selectedEntity;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}

void EditWindowUI::sendEntityCloneEvent(Identity::Type entityId) {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::CLONE;
    evt->m_entityId = entityId;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}

void EditWindowUI::sendComponentCreationEvent(std::string name) {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::CREATE_COMPONENT;
    evt->m_name = std::move(name);
    evt->m_entityId = m_selectedEntity;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}
void EditWindowUI::sendComponentRemovalEvent(std::string name) {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::REMOVE_COMPONENT;
    evt->m_name = std::move(name);
    evt->m_entityId = m_selectedEntity;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}
