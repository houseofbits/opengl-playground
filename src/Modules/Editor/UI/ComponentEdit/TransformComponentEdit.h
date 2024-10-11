#pragma once

#include "../../../Common/Components/TransformComponent.h"
#include "BaseComponentEdit.h"

class TransformComponentEdit : public BaseComponentEdit {
public:
    explicit TransformComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<TransformComponent>(entityId);
    }

    std::string getName() override {
        return "Transform";
    }

    void process(Identity::Type entityId) override {
        auto *transform = m_EditorUISystem->getComponent<TransformComponent>(entityId);
        if (transform == nullptr) {
            return;
        }

        float matrixTranslation[3], matrixRotation[3], matrixScale[3];

        ImGuizmo::DecomposeMatrixToComponents(&transform->m_transform[0][0], matrixTranslation, matrixRotation, matrixScale);
        if (transform->m_isTranslationEnabled) {
            ImGui::InputFloat3("Translation", matrixTranslation);
        }
        if (transform->m_isRotationEnabled) {
            ImGui::InputFloat3("Rotation", matrixRotation);
        }
        if (transform->m_isScalingEnabled) {
            ImGui::InputFloat3("Scaling", matrixScale);
        }
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &transform->m_transform[0][0]);


        if (ImGui::BeginCombo("Parent##PARENT_ENTITY_NAME", transform->m_parentEntityName.c_str())) {
            for (const auto &entity: m_EditorUISystem->m_EntityContext->getAllEntities()) {
                auto* c = entity->getComponent<TransformComponent>();
                if(c != nullptr && c->m_EntityId.id() != transform->m_EntityId.id()) {
                    if (ImGui::Selectable(entity->m_Name.c_str(), entity->m_Name == transform->m_parentEntityName)) {
                        transform->m_parentEntityName = entity->m_Name;
                        transform->m_shouldUpdateParentEntityId = true;
                    }
                }
            }
            if (ImGui::Selectable("No parent", transform->m_parentEntityName.empty())) {
                transform->m_parentEntityName.clear();
                transform->m_shouldUpdateParentEntityId = true;
            }
            ImGui::EndCombo();
        }
    }
};
