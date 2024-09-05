#pragma once

#include "../../../../SourceLibs/imgui/imgui.h"
#include "../FileDialogHelper.h"
#include "../TextPromptHelper.h"
#include "BaseComponentEdit.h"

class PhysicsBodyComponentEdit : public BaseComponentEdit {
public:
    inline static std::map<PhysicsBodyComponent::BodyType, std::string> BodyTypeNameMap = {
            {PhysicsBodyComponent::BODY_TYPE_STATIC, "Static"},
            {PhysicsBodyComponent::BODY_TYPE_DYNAMIC, "Dynamic"},
    };

    inline static std::map<PhysicsBodyComponent::MeshType, std::string> MeshTypeNameMap = {
            {PhysicsBodyComponent::MESH_TYPE_CONVEX, "Convex mesh"},
            {PhysicsBodyComponent::MESH_TYPE_TRIANGLE, "Triangle mesh"},
    };

    explicit PhysicsBodyComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<PhysicsBodyComponent>(entityId);
    }

    std::string getName() override {
        return "Physics body";
    }

    void process(Identity::Type entityId) override {
        auto *body = m_EditorUISystem->getComponent<PhysicsBodyComponent>(entityId);
        if (body == nullptr) {
            return;
        }

        ImGui::SeparatorText("Physics body");

        if (ImGui::BeginCombo("Body type##PHYSICS_BODY_TYPE", BodyTypeNameMap[body->m_BodyType].c_str())) {
            for (const auto &bodyType: BodyTypeNameMap) {
                if (ImGui::Selectable(bodyType.second.c_str(), body->m_BodyType == bodyType.first)) {
                    body->m_BodyType = bodyType.first;
                    auto *transform = m_EditorUISystem->getComponent<TransformComponent>(entityId);
                    body->create(*transform);
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Mesh type##COLLISION_MESH_TYPE", MeshTypeNameMap[body->m_MeshType].c_str())) {
            for (const auto &meshType: MeshTypeNameMap) {
                if (ImGui::Selectable(meshType.second.c_str(), body->m_MeshType == meshType.first)) {
                    body->m_MeshType = meshType.first;
                    auto *transform = m_EditorUISystem->getComponent<TransformComponent>(entityId);
                    body->createMeshShape(*transform);
                }
            }
            ImGui::EndCombo();
        }

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
