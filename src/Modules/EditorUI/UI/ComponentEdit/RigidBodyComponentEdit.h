#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class RigidBodyComponentEdit : public BaseComponentEdit {
public:
    explicit RigidBodyComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<RigidBodyComponent>(entityId);
    }

    std::string getName() override {
        return "Rigid body";
    }

    void process(Identity::Type entityId) override {
        auto *body = m_EditorUISystem->getComponent<RigidBodyComponent>(entityId);
        if (body == nullptr) {
            return;
        }

        ImGui::SeparatorText("Rigid body");

        if (FileInput("ChooseRigidBodyMeshFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath, body->m_meshResource().m_Path)) {
            body->m_meshResource.invalidate();
            m_EditorUISystem->m_ResourceManager->request(body->m_meshResource, m_meshPath);
        }

        if (ImGui::Button("Create from render mesh")) {
            auto *mesh = m_EditorUISystem->getComponent<StaticMeshComponent>(entityId);
            if(mesh && mesh->m_Mesh.isReady()) {
                body->m_meshResource.invalidate();
                m_EditorUISystem->m_ResourceManager->request(body->m_meshResource, mesh->m_Mesh.get().m_Path);
            }
        }

        if (ImGui::Button("Rebuild shape")) {
            auto *transform = m_EditorUISystem->getComponent<TransformComponent>(entityId);
            body->createMeshShape(*transform);
        }

        ImGui::InputFloat("Density", &body->m_density);
        ImGui::InputFloat("Dynamic friction", &body->m_friction.x);
        ImGui::InputFloat("Static friction", &body->m_friction.y);
        ImGui::InputFloat("Restitution", &body->m_restitution);
    }

    std::string m_meshPath;
};
