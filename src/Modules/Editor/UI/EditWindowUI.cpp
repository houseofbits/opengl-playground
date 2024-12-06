#include "EditWindowUI.h"
#include "../../../Core/Events/EntityCreationEvent.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../../Physics/Components/PhysicsBodyComponent.h"
#include "../../Physics/Components/PhysicsHingeJointComponent.h"
#include "../Systems/EditorUISystem.h"
#include "../../../Core/Helper/StringUtils.h"
#include "../../Physics/Components/PhysicsFixedJointComponent.h"
#include "../../Physics/Components/PhysicsSliderJointComponent.h"
#include <utility>

EditWindowUI::EditWindowUI(EditorUISystem *editor) : m_selectedEntityId(0),
                                                     m_EditorUISystem(editor),
                                                     m_lightProjectorPath(),
                                                     m_isBoundsTransformAllowed(false),
                                                     m_meshModelPath(),
                                                     m_meshMaterialPath(),
                                                     m_selectedEntityCreationType(),
                                                     m_selectedComponentCreationType(),
                                                     m_selectedBehaviour(0)
                                                     {
}

void EditWindowUI::process() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (ImGui::Begin("Entities")) {

        processEntityCreation();

        ImGui::PushItemWidth(-FLT_MIN);
        if (ImGui::InputText("##FILTER_STRING", &m_filterString)) {
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

        for (const auto &component: e->m_Components) {
            auto editor = m_EditorUISystem->m_componentEditors[component->getTypeName()];
            if (editor && ImGui::CollapsingHeader(editor->getName().c_str())) {
                editor->process(*e, *m_EditorUISystem);
            }
        }

        if (ImGui::CollapsingHeader("Behaviours")) {
            processBehavioursEdit(e);
        }

        processComponentCreation();

        ImGui::End();
    }
}

void EditWindowUI::processEntitiesList() {
    if (ImGui::BeginListBox("##ENTITY_LIST", ImVec2(-1, -1))) {
        for (const auto &entity: m_EditorUISystem->m_EntityContext->getAllEntities()) {
            std::string name = entity->m_Name;
            bool isEditable = true;
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
    if (m_selectedEntityId < 0) {
        return false;
    }

    if (!m_EditorUISystem->m_EntityContext->getEntityComponent<TransformComponent>(m_selectedEntityId)) {
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

void EditWindowUI::processComponentCreation() {
    ImGui::SeparatorText("Add component");

    ImGui::PushItemWidth(150);

    std::string selectedName;

    if (m_EditorUISystem->m_componentEditors.find(m_selectedComponentCreationType) != m_EditorUISystem->m_componentEditors.end()) {
        selectedName = m_EditorUISystem->m_componentEditors[m_selectedComponentCreationType]->getName();
    }

    if (ImGui::BeginCombo("##COMPONENT_TYPE", selectedName.c_str())) {
        for (auto const &[componentTypeName, editor]: m_EditorUISystem->m_componentEditors) {
            if (ImGui::Selectable(
                    editor->getName().c_str(), m_selectedComponentCreationType == componentTypeName)) {
                m_selectedComponentCreationType = componentTypeName;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::Button("Add")) {
        sendComponentCreationEvent(m_selectedComponentCreationType);
    }

    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        sendComponentRemovalEvent(m_selectedComponentCreationType);
    }
}

void EditWindowUI::processEntityCreation() {

    auto names = m_EditorUISystem->m_EntityContext->getAllConfigurationNames();

    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##ENTITY_TYPE", m_selectedEntityCreationType.c_str())) {
        for (const auto &name: names) {
            if (ImGui::Selectable(name.c_str(), m_selectedEntityCreationType == name)) {
                m_selectedEntityCreationType = name;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::Button("Create")) {
        sendEntityCreationEvent(m_selectedEntityCreationType,
                                "NEW " + m_selectedEntityCreationType);
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
}
