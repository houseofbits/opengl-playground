#pragma once

#include "../Components/TransformComponent.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class TransformComponentEdit : public BaseComponentEdit {
public:
    std::string getName() override {
        return "Transform";
    }

    void process(Entity &entity, EditorUISystem& system) {
        auto *transform = entity.getComponent<TransformComponent>();
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

//        std::string selectedName;
//        if (transform->m_parentEntityId > 0) {
//            auto *e = m_EditorUISystem->m_EntityContext->getEntity(transform->m_parentEntityId);
//            selectedName = e->m_Name;
//        }
        if (ImGui::BeginCombo("Parent##PARENT_ENTITY_NAME", transform->m_parentEntityName.c_str())) {
            for (const auto &e: system.m_EntityContext->getAllEntities()) {
                auto* c = e->getComponent<TransformComponent>();
                if(c != nullptr && c->m_EntityId.id() != transform->m_EntityId.id()) {
                    if (ImGui::Selectable(e->m_Name.c_str(), e->m_Name == transform->m_parentEntityName)) {
                        transform->m_parentEntityName = e->m_Name;
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
