
#pragma once

#include "../../../Behaviour/Components/EntityBehaviourComponent.h"
#include "BaseComponentEdit.h"

class EntityBehaviourComponentEdit : public BaseComponentEdit {
public:
    explicit EntityBehaviourComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<EntityBehaviourComponent>(entityId);
    }

    std::string getName() override {
        return "Behaviour";
    }

    void process(Identity::Type entityId) override {
        auto *component = m_EditorUISystem->getComponent<EntityBehaviourComponent>(entityId);
        if (component == nullptr) {
            return;
        }

        if (ImGui::BeginListBox("##BEHAVIOUR_LIST", ImVec2(-1, 100))) {
            for (const auto &behaviour: component->m_behaviours) {
                std::string isEnabled = behaviour.first ? "enabled" : "disabled";
                std::string name =
                        EntityBehaviourComponent::BehaviourNames.at(behaviour.second) + " [" + isEnabled + "]";
                if (ImGui::Selectable(name.c_str(), m_listboxSelectedBehaviour == behaviour.second)) {
                    m_listboxSelectedBehaviour = behaviour.second;
                }
            }

            ImGui::EndListBox();
        }

        if (ImGui::Button("Remove##REMOVE_BEHAVIOUR")) {
            component->removeBehaviour(m_listboxSelectedBehaviour);
        }

        if (ImGui::BeginCombo("##BEHAVIOUR",
                              EntityBehaviourComponent::BehaviourNames.at(m_selectedBehaviour).c_str())) {
            for (const auto &behaviour: EntityBehaviourComponent::BehaviourNames) {
                if (ImGui::Selectable(behaviour.second.c_str(), m_selectedBehaviour == behaviour.first)) {
                    m_selectedBehaviour = behaviour.first;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (ImGui::Button("Add##ADD_BEHAVIOUR")) {
            component->addBehaviour(m_selectedBehaviour);
        }

        ImGui::TextWrapped("%s", EntityBehaviourComponent::BehaviourDescriptions.at(m_selectedBehaviour).c_str());
    }

    EntityBehaviourComponent::BehaviourType m_selectedBehaviour;
    EntityBehaviourComponent::BehaviourType m_listboxSelectedBehaviour;
};
