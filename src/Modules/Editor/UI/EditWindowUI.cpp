#include "EditWindowUI.h"
#include "../../../Core/Events/EntityCreationEvent.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../../Physics/Components/PhysicsBodyComponent.h"
#include "../../Physics/Components/PhysicsHingeJointComponent.h"
#include "../Systems/EditorUISystem.h"
#include "ComponentEdit/CameraComponentEdit.h"
#include "ComponentEdit/CharacterControllerComponentEdit.h"
#include "ComponentEdit/EnvironmentProbeComponentEdit.h"
#include "ComponentEdit/LightComponentEdit.h"
#include "ComponentEdit/PhysicsBodyComponentEdit.h"
#include "ComponentEdit/PhysicsHingeJointComponentEdit.h"
#include "ComponentEdit/StaticMeshComponentEdit.h"
#include "ComponentEdit/TransformComponentEdit.h"
#include "../../../Core/Helper/StringUtils.h"
#include "../../Physics/Components/PhysicsFixedJointComponent.h"
#include "ComponentEdit/PhysicsFixedJointComponentEdit.h"
#include "../../Physics/Components/PhysicsSliderJointComponent.h"
#include "ComponentEdit/PhysicsSliderJointComponentEdit.h"
#include <utility>

std::vector<std::string> ENTITY_CREATION_OPTIONS = {
        "RenderMesh",
        "RigidBody",
        "SpotLight",
        "OmniLight",
        "DirectLight",
        "EnvironmentProbe",
        "CharacterController",
        "Door"
};

std::vector<std::pair<std::string, std::string>> COMPONENT_NAMES = {
        {"light",             "Light"},
        {"camera",            "Camera"},
        {"mesh",              "Render mesh"},
        {"probe",             "Environment probe"},
        {"sky",               "Skybox"},
        {"cct",               "Character"},
        {"physicsBody",       "Physics body"},
        {"physicsHingeJoint", "Physics hinge joint"},
        {"physicsSliderJoint", "Physics slider joint"},
        {"physicsFixedJoint", "Physics fixed joint"},
};

EditWindowUI::EditWindowUI(EditorUISystem *editor) : m_selectedEntityId(0),
                                                     m_EditorUISystem(editor),
                                                     m_lightProjectorPath(),
                                                     m_isBoundsTransformAllowed(false),
                                                     m_meshModelPath(),
                                                     m_meshMaterialPath(),
                                                     m_selectedEntityCreationType(0),
                                                     m_selectedComponentCreationType(0),
                                                     m_selectedBehaviour(0),
                                                     m_componentEditors(),
                                                     m_entityListFilter() {

    m_componentEditors[TransformComponent::TypeName()] = new TransformComponentEdit(editor);
    m_componentEditors[StaticMeshComponent::TypeName()] = new StaticMeshComponentEdit(editor);
    m_componentEditors[LightComponent::TypeName()] = new LightComponentEdit(editor);
    m_componentEditors[CameraComponent::TypeName()] = new CameraComponentEdit(editor);
    m_componentEditors[EnvironmentProbeComponent::TypeName()] = new EnvironmentProbeComponentEdit(editor);
    m_componentEditors[PhysicsBodyComponent::TypeName()] = new PhysicsBodyComponentEdit(editor);
    m_componentEditors[PhysicsCharacterComponent::TypeName()] = new CharacterControllerComponentEdit(editor);
    m_componentEditors[PhysicsHingeJointComponent::TypeName()] = new PhysicsHingeJointComponentEdit(editor);
    m_componentEditors[PhysicsFixedJointComponent::TypeName()] = new PhysicsFixedJointComponentEdit(editor);
    m_componentEditors[PhysicsSliderJointComponent::TypeName()] = new PhysicsSliderJointComponentEdit(editor);

    for (const auto &edit: m_componentEditors) {
        m_entityListFilter[edit.first] = true;
    }
}

void EditWindowUI::process() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (ImGui::Begin("Entities")) {
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
            sendEntityCreationEvent(ENTITY_CREATION_OPTIONS[m_selectedEntityCreationType],
                                    "NEW " + ENTITY_CREATION_OPTIONS[m_selectedEntityCreationType]);
        }

        if (m_selectedEntityId >= 0) {
            ImGui::SameLine();
            if (ImGui::Button("Clone")) {
                sendEntityCloneEvent(m_selectedEntityId);
            }

            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                sendEntityRemovalEvent();
                m_selectedEntityId = -1;
            }
        }

        ImGui::PushItemWidth(-FLT_MIN);
        if (ImGui::InputText("##FILTER_STRING", &m_filterString)) {
        }

        if (ImGui::BeginCombo("##ENTITY_FILTER", "Filter")) {
            for (const auto &edit: m_componentEditors) {
                ImGui::Checkbox(edit.second->getName().c_str(), &m_entityListFilter[edit.first]);
            }
            m_selectedEntityId = -1;
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        processEntitiesList();

        ImGui::End();
    }
    ImGui::PopStyleVar();

    Entity *e = m_EditorUISystem->m_EntityContext->getEntity(m_selectedEntityId);
    if (e != nullptr && ImGui::Begin("Edit entity")) {
        if (ImGui::InputText("Name", &e->m_Name)) {
            updateEntityNameReferences(e->m_Id.id(), e->m_Name);
        }

        for (const auto &edit: m_componentEditors) {
            if (edit.second->isEntityEditable(m_selectedEntityId) &&
                ImGui::CollapsingHeader(edit.second->getName().c_str())) {
                edit.second->process(m_selectedEntityId);
            }
        }

        if (ImGui::CollapsingHeader("Behaviours")) {
            processBehavioursEdit(e);
        }

        ImGui::SeparatorText("Add component");

        ImGui::PushItemWidth(150);
        if (ImGui::BeginCombo("##COMPONENT_TYPE",
                              COMPONENT_NAMES[m_selectedComponentCreationType].second.c_str())) {
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

        ImGui::End();
    }
}

void EditWindowUI::processEntitiesList() {
    if (ImGui::BeginListBox("##ENTITY_LIST", ImVec2(-1, -1))) {
        for (const auto &entity: m_EditorUISystem->m_EntityContext->getAllEntities()) {
            std::string name = entity->m_Name;

            bool isEditable = false;
            for (const auto &edit: m_componentEditors) {
                if (edit.second->isEntityEditable(entity->m_Id.id()) && m_entityListFilter[edit.first]) {
                    isEditable = true;
                    break;
                }
            }

            if (!m_filterString.empty()) {
                if (StringUtils::toLowerCase(entity->m_Name).find(StringUtils::toLowerCase(m_filterString)) !=
                    std::string::npos) {
                    isEditable = true;
                } else {
                    isEditable = false;
                }
            }

            if (isEditable && ImGui::Selectable(name.c_str(), m_selectedEntityId == entity->m_Id.id())) {
                m_selectedEntityId = (int) entity->m_Id.id();
            }
        }

        ImGui::EndListBox();
    }
}

bool EditWindowUI::isTransformComponentSelected() {
    if (m_selectedEntityId < 0 || m_EditorUISystem->getComponent<TransformComponent>(m_selectedEntityId) == nullptr) {
        return false;
    }

    return true;
}

void EditWindowUI::sendEntityCreationEvent(std::string configName, std::string entityName) {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::CREATE,
                                                                      std::move(entityName), std::move(configName));
}

void EditWindowUI::sendEntityRemovalEvent() {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::REMOVE,
                                                                      m_selectedEntityId);
}

void EditWindowUI::sendEntityCloneEvent(Identity::Type entityId) {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::CLONE, entityId);
}

void EditWindowUI::sendComponentCreationEvent(std::string name) {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::CREATE_COMPONENT,
                                                                      m_selectedEntityId, std::move(name));
}

void EditWindowUI::sendComponentRemovalEvent(std::string name) {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::REMOVE_COMPONENT,
                                                                      m_selectedEntityId, std::move(name));
}

void EditWindowUI::processBehavioursEdit(Entity *e) {
    if (ImGui::BeginCombo("##BEHAVIOUR", "Add")) {
        for (const auto &behaviourType: m_EditorUISystem->m_EntityContext->getBehaviourTypes()) {
            if (ImGui::Selectable(behaviourType.c_str(), false)) {
                m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::ADD_BEHAVIOUR,
                                                                                  m_selectedEntityId, behaviourType);

            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginListBox("##BEHAVIOUR_LIST", ImVec2(-1, 100))) {
        for (const auto &behaviour: e->m_Behaviours) {
            std::string name = behaviour->getDisplayName();

            if (ImGui::Selectable(name.c_str(), m_selectedBehaviour == behaviour->getTypeId())) {
                m_selectedBehaviour = (int) behaviour->getTypeId();
            }
        }

        ImGui::EndListBox();
    }
    if (m_selectedBehaviour > 0 && ImGui::Button("Remove##REMOVE_BEHAVIOUR")) {
        auto it = std::find_if(
                e->m_Behaviours.begin(),
                e->m_Behaviours.end(),
                [&](EntityBehaviour *p) {
                    return p->getTypeId() == m_selectedBehaviour;
                });

        if (it != e->m_Behaviours.end()) {
            m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::REMOVE_BEHAVIOUR,
                                                                              m_selectedEntityId,
                                                                              (*it)->getTypeName());
        }
    }

    if (m_selectedBehaviour > 0) {
        auto it = std::find_if(
                e->m_Behaviours.begin(),
                e->m_Behaviours.end(),
                [&](EntityBehaviour *p) {
                    return p->getTypeId() == m_selectedBehaviour;
                });
        if (it != e->m_Behaviours.end()) {
            ImGui::Text("Type: %s", (*it)->getTypeName().c_str());
            ImGui::TextWrapped("Description: %s", (*it)->getDescription().c_str());
        }
    }
}

void EditWindowUI::updateEntityNameReferences(Identity::Type entityId, const std::string &newName) {
    for (const auto &transformComponent: m_EditorUISystem->getComponentContainer<TransformComponent>()) {
        if (transformComponent.second->m_parentEntityId == entityId) {
            transformComponent.second->m_parentEntityName = newName;
        }
    }

    for (const auto &jointComponent: m_EditorUISystem->getComponentContainer<PhysicsHingeJointComponent>()) {
        if (jointComponent.second->m_targetEntityId == entityId) {
            jointComponent.second->m_targetEntityName = newName;
        }
    }

    for (const auto &jointComponent: m_EditorUISystem->getComponentContainer<PhysicsFixedJointComponent>()) {
        if (jointComponent.second->m_targetEntityId == entityId) {
            jointComponent.second->m_targetEntityName = newName;
        }
    }

    for (const auto &jointComponent: m_EditorUISystem->getComponentContainer<PhysicsSliderJointComponent>()) {
        if (jointComponent.second->m_targetEntityId == entityId) {
            jointComponent.second->m_targetEntityName = newName;
        }
    }
}
