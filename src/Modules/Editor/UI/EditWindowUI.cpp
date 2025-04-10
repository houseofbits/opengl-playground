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
#include <algorithm>

EditWindowUI::EditWindowUI(EditorUISystem *editor) : //m_selectedEntityId(0),
    m_EditorUISystem(editor),
    m_lightProjectorPath(),
    m_isBoundsTransformAllowed(false),
    m_meshModelPath(),
    m_meshMaterialPath(),
    m_selectedEntityCreationType(),
    m_selectedComponentCreationType(),
    m_isComponentCreationOpen(false),
    m_selectedComponents{},
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
    if (e != nullptr && ImGui::Begin("Edit entity")) {
        if (ImGui::InputText("Name", &e->m_Name)) {
            updateEntityNameReferences(e->m_Id.id(), e->m_Name);
        }

        if (m_isComponentCreationOpen) {
            processComponentCreation(e);

            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            ImGui::Separator();
            if (ImGui::Button("Back to Edit entity")) {
                m_isComponentCreationOpen = false;
            }
        } else {
            processComponentEditors(e);

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            ImGui::Separator();
            if (ImGui::Button("Add/remove components")) {
                clearComponentSelection();
                m_isComponentCreationOpen = true;
            }
        }

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

void EditWindowUI::processComponentEditors(Entity *e) const {
    int headerId = 1;
    for (const auto &component: e->m_Components) {
        std::string headerIdStr = "COMPONENT_HEADER_" + std::to_string(headerId);
        std::string headerText = component->getHumanReadableTypeName() + "###" + headerIdStr;

        if (ImGui::CollapsingHeader(headerText.c_str())) {
            std::string internalId = "COMPONENT_SECTION_" + std::to_string(headerId);
            ImGui::PushID(internalId.c_str());

            ImGui::InputText("Editor component name", &component->m_Name);

            m_EditorUISystem->runProcessComponentEditor(e, component.get());

            ImGui::PopID();
        }

        headerId++;
    }
}

bool EditWindowUI::isTransformComponentSelected() const {
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

void EditWindowUI::sendComponentRemovalEvent(const int componentId) const {
    m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::REMOVE_COMPONENT,
                                                                      m_EditorUISystem->getSelectedEntity()->m_Id.id(),
                                                                      componentId);
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

void EditWindowUI::processComponentCreation(Entity *entity) {
    ImGui::SeparatorText("Delete components");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    bool hasSelectedComponents = false;
    int index = 0;
    for (const auto comp: entity->m_Components) {
        ImGui::Selectable(comp->getHumanReadableTypeName().c_str(), &m_selectedComponents[index]);

        if (m_selectedComponents[index]) {
            hasSelectedComponents = true;
        }
        index++;
    }

    if (hasSelectedComponents) {
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
    }

    if (hasSelectedComponents && ImGui::Button("Delete")) {
        index = 0;
        for (const auto comp: entity->m_Components) {
            if (m_selectedComponents[index]) {
                sendComponentRemovalEvent(comp->m_Id.id());
            }
            index++;
        }

        clearComponentSelection();
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::SeparatorText("Add component");

    if (ImGui::BeginListBox("##COMPONENT_CREATION_LIST", ImVec2(-1, 350))) {
        for (const auto componentTypeName: m_EditorUISystem->m_EntityContext->getAllComponentTypeNames()) {
            if (ImGui::Selectable(componentTypeName.c_str(), m_selectedComponentCreationType == componentTypeName)) {
                m_selectedComponentCreationType = componentTypeName;
            }
        }
        ImGui::EndListBox();
    }

    if (!m_selectedComponentCreationType.empty()) {
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
    }

    if (!m_selectedComponentCreationType.empty() && ImGui::Button("Add")) {
        sendComponentCreationEvent(m_selectedComponentCreationType);
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
            const auto entityId = m_EditorUISystem->getSelectedEntity()->m_Id.id();
            m_EditorUISystem->clearEntitySelection();
            m_EditorUISystem->m_EventManager->queueEvent<EntityCreationEvent>(EntityCreationEvent::REMOVE, entityId);
        }

        ImGui::SameLine();
    }

    if (ImGui::Button("Create")) {
        m_EditorUISystem->clearEntitySelection();
        m_isEntityCreationWindowOpen = true;
    }
}

void EditWindowUI::clearComponentSelection() {
    std::fill_n(m_selectedComponents, 30, false);
    m_selectedComponentCreationType = "";
}
