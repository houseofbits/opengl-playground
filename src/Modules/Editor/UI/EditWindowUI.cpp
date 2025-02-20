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

EditWindowUI::EditWindowUI(EditorUISystem *editor) : //m_selectedEntityId(0),
    m_EditorUISystem(editor),
    m_lightProjectorPath(),
    m_isBoundsTransformAllowed(false),
    m_meshModelPath(),
    m_meshMaterialPath(),
    m_selectedEntityCreationType(),
    m_selectedComponentCreationType(),
    m_isEntityCreationWindowOpen(false) {
}

void EditWindowUI::process() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    if (ImGui::Begin("Entities")) {
        processEntityCreation();

        ImGui::PushItemWidth(-FLT_MIN);
        if (ImGui::InputText("##FILTER_STRING", &m_entitiesFilterString)) {
        }
        ImGui::PopItemWidth();

        processEntitiesList();

        ImGui::End();
    }
    ImGui::PopStyleVar();

    Entity *e = m_EditorUISystem->getSelectedEntity();
    //_EntityContext->getEntity(m_EditorUISystem->m_selectedEntityId);
    if (e != nullptr && ImGui::Begin("Edit entity")) {
        if (ImGui::InputText("Name", &e->m_Name)) {
            updateEntityNameReferences(e->m_Id.id(), e->m_Name);
        }

        for (const auto &component: e->m_Components) {
            if (auto editor = m_EditorUISystem->m_componentEditors[component->getTypeName()]) {
                if (ImGui::CollapsingHeader(editor->getName().c_str())) {
                    editor->processEditor();
                }
            } else {
                if (ImGui::CollapsingHeader(component->getTypeName().c_str())) {
                    ImGui::Text("Editor not registered for: %s ", component->getTypeName().c_str());
                }
            }
        }

        processComponentCreation();

        ImGui::End();
    }

    if (m_isEntityCreationWindowOpen && ImGui::Begin("Create entity", &m_isEntityCreationWindowOpen)) {
        processEntityCreationWindow();

        ImGui::End();
    }
}

void EditWindowUI::processEntitiesList() {
    if (ImGui::BeginListBox("##ENTITY_LIST", ImVec2(-1, -1))) {
        for (const auto &entity: m_EditorUISystem->m_EntityContext->getAllEntities()) {
            std::string name = entity->m_Name;
            bool isEditable =
                    m_entitiesFilterString.empty() || StringUtils::contains(entity->m_Name, m_entitiesFilterString);

            if (isEditable && ImGui::Selectable(name.c_str(),
                                                m_EditorUISystem->getSelectedEntity() == entity.get())) {
                m_EditorUISystem->selectEntity(*entity); //m_selectedEntityId = (int) entity->m_Id.id();
                m_isEntityCreationWindowOpen = false;
            }
        }

        ImGui::EndListBox();
    }
}

bool EditWindowUI::isTransformComponentSelected() {
    if (m_EditorUISystem->getSelectedEntity() == nullptr) {
        return false;
    }

    if (!m_EditorUISystem->getSelectedEntity()->getComponent<TransformComponent>()) {
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
                                                                      m_EditorUISystem->getSelectedEntity()->m_Id.id()
    );
}

void EditWindowUI::sendEntityCloneEvent(Identity::Type entityId) {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::CLONE, entityId);
}

void EditWindowUI::sendComponentCreationEvent(std::string name) {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::CREATE_COMPONENT,
                                                                      m_EditorUISystem->getSelectedEntity()->m_Id.id(),
                                                                      std::move(name));
}

void EditWindowUI::sendComponentRemovalEvent(std::string name) {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::REMOVE_COMPONENT,
                                                                      m_EditorUISystem->getSelectedEntity()->m_Id.id(),
                                                                      std::move(name));
}

void EditWindowUI::updateEntityNameReferences(Identity::Type entityId, const std::string &newName) {
    //TODO: Add event type for name changes
    //    for (const auto &transformComponent: m_EditorUISystem->getComponentContainer<TransformComponent>()) {
    //        if (transformComponent.second->m_parentEntityId == entityId) {
    //            transformComponent.second->m_parentEntityName = newName;
    //        }
    //    }
    //
    //    for (const auto &jointComponent: m_EditorUISystem->getComponentContainer<PhysicsHingeJointComponent>()) {
    //        if (jointComponent.second->m_targetEntityId == entityId) {
    //            jointComponent.second->m_targetEntityName = newName;
    //        }
    //    }
    //
    //    for (const auto &jointComponent: m_EditorUISystem->getComponentContainer<PhysicsFixedJointComponent>()) {
    //        if (jointComponent.second->m_targetEntityId == entityId) {
    //            jointComponent.second->m_targetEntityName = newName;
    //        }
    //    }
    //
    //    for (const auto &jointComponent: m_EditorUISystem->getComponentContainer<PhysicsSliderJointComponent>()) {
    //        if (jointComponent.second->m_targetEntityId == entityId) {
    //            jointComponent.second->m_targetEntityName = newName;
    //        }
    //    }
}

void EditWindowUI::processComponentCreation() {
    ImGui::SeparatorText("Add component");

    ImGui::PushItemWidth(150);

    std::string selectedName;

    if (m_EditorUISystem->m_componentEditors.find(m_selectedComponentCreationType) !=
        m_EditorUISystem->m_componentEditors.end()) {
        selectedName = m_EditorUISystem->m_componentEditors[m_selectedComponentCreationType]->getName();
    }

    if (ImGui::BeginCombo("##COMPONENT_TYPE", selectedName.c_str())) {
        for (auto const &[componentTypeName, editor]: m_EditorUISystem->m_componentEditors) {
            //     if (ImGui::Selectable(
            //         editor->getName().c_str(), m_selectedComponentCreationType == componentTypeName)) {
            //         m_selectedComponentCreationType = componentTypeName;
            //     }
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

void EditWindowUI::processEntityCreationWindow() {
    if (ImGui::Button("Create")) {
        sendEntityCreationEvent(m_selectedEntityCreationType,
                                "NEW " + m_selectedEntityCreationType);
    }

    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::InputText("##TEMPLATE_FILTER_STRING", &m_templatesFilterString)) {
    }
    ImGui::PopItemWidth();

    auto names = m_EditorUISystem->m_EntityContext->getAllConfigurationNames();
    if (ImGui::BeginListBox("##ENTITY_CREATION_LIST", ImVec2(-1, -1))) {
        for (const auto &name: names) {
            bool isEditable =
                    m_templatesFilterString.empty() || StringUtils::contains(name, m_templatesFilterString);

            if (isEditable && ImGui::Selectable(name.c_str(), m_selectedEntityCreationType == name)) {
                m_selectedEntityCreationType = name;
            }
        }
        ImGui::EndListBox();
    }
}

void EditWindowUI::processEntityCreation() {
    auto e = m_EditorUISystem->getSelectedEntity();
    if (e) {
        if (ImGui::Button("Clone")) {
            sendEntityCloneEvent(e->m_Id.id());
        }

        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            sendEntityRemovalEvent();
            m_EditorUISystem->clearEntitySelection();
        }

        ImGui::SameLine();
    }

    if (ImGui::Button("Create")) {
        m_EditorUISystem->clearEntitySelection();
        m_isEntityCreationWindowOpen = true;
    }
}
