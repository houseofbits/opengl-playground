#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class PhysicsMeshComponentEdit : public BaseComponentEdit {
public:
    inline static std::map<PhysicsMeshComponent::MeshType, std::string> MeshTypeNameMap = {
            {PhysicsMeshComponent::CONVEX_MESH_TYPE, "Convex mesh"},
            {PhysicsMeshComponent::TRIANGLE_MESH_TYPE, "Triangle mesh"},
    };

    explicit PhysicsMeshComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<PhysicsMeshComponent>(entityId);
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

        if (ImGui::BeginCombo("Type##COLLISION_MESH_TYPE", MeshTypeNameMap[mesh->m_MeshType].c_str())) {
            for (const auto &meshType: MeshTypeNameMap) {
                if (ImGui::Selectable(meshType.second.c_str(), mesh->m_MeshType == meshType.first)) {
                    mesh->m_MeshType = meshType.first;
                    auto *transform = m_EditorUISystem->getComponent<TransformComponent>(entityId);
                    mesh->createMeshShape(*transform);
                }
            }
            ImGui::EndCombo();
        }

        if (FileInput("ChooseCollisionMeshMeshFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath, mesh->m_meshResource().m_Path)) {
            mesh->m_meshResource.invalidate();
            m_EditorUISystem->m_ResourceManager->request(mesh->m_meshResource, m_meshPath);
        }

        if (ImGui::Button("Create from render mesh")) {
            auto *renderMesh = m_EditorUISystem->getComponent<StaticMeshComponent>(entityId);
            if (renderMesh && renderMesh->m_Mesh.isReady()) {
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
