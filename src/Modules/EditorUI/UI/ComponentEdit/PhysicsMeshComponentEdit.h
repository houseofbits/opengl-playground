#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class PhysicsMeshComponentEdit : public BaseComponentEdit {
public:
    explicit PhysicsMeshComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<PhysicsMeshComponentEdit>(entityId);
    }

    std::string getName() override {
        return "Collision mesh";
    }

    void process(Identity::Type entityId) override {
        auto *mesh = m_EditorUISystem->getComponent<PhysicsMeshComponent>(entityId);
        if (mesh == nullptr) {
            return;
        }

        ImGui::SeparatorText("Collision mesh");

        if (FileInput("ChooseCollisionMeshMeshFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath, mesh->m_meshResource().m_Path)) {
            mesh->m_meshResource.invalidate();
            m_EditorUISystem->m_ResourceManager->request(mesh->m_meshResource, m_meshPath);
        }

        if (ImGui::Button("Create from render mesh")) {
            auto *renderMesh = m_EditorUISystem->getComponent<StaticMeshComponent>(entityId);
            if(renderMesh && renderMesh->m_Mesh.isReady()) {
                mesh->m_meshResource.invalidate();
                m_EditorUISystem->m_ResourceManager->request(mesh->m_meshResource, renderMesh->m_Mesh.get().m_Path);
            }
        }

        if (ImGui::Button("Rebuild shape")) {
            auto *transform = m_EditorUISystem->getComponent<TransformComponent>(entityId);
            mesh->createMeshShape(*transform);
        }

        ImGui::InputFloat("Dynamic friction", &mesh->m_friction.x);
        ImGui::InputFloat("Static friction", &mesh->m_friction.y);
        ImGui::InputFloat("Restitution", &mesh->m_restitution);
    }

    std::string m_meshPath;
};
