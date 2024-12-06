#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"

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

    std::string getName() override {
        return "Physics body";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *body = entity.getComponent<PhysicsBodyComponent>();
        if (body == nullptr) {
            return;
        }

        if (ImGui::BeginCombo("Body type##PHYSICS_BODY_TYPE", BodyTypeNameMap[body->m_BodyType].c_str())) {
            for (const auto &bodyType: BodyTypeNameMap) {
                if (ImGui::Selectable(bodyType.second.c_str(), body->m_BodyType == bodyType.first)) {
                    body->m_BodyType = bodyType.first;
                    body->release();
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Mesh type##COLLISION_MESH_TYPE", MeshTypeNameMap[body->m_MeshType].c_str())) {
            for (const auto &meshType: MeshTypeNameMap) {
                if (ImGui::Selectable(meshType.second.c_str(), body->m_MeshType == meshType.first)) {
                    body->m_MeshType = meshType.first;
                    body->release();
                }
            }
            ImGui::EndCombo();
        }

        if (FileInput("ChooseRigidBodyMeshFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath, body->m_meshResource().m_Path)) {
            body->m_meshResource.invalidate();
            system.m_ResourceManager->request(body->m_meshResource, m_meshPath);
        }

        if (ImGui::Button("Create from render mesh")) {
            auto *mesh = entity.getComponent<StaticMeshComponent>();
            if(mesh && mesh->m_Mesh.isReady()) {
                body->m_meshResource.invalidate();
                system.m_ResourceManager->request(body->m_meshResource, mesh->m_Mesh.get().m_Path);
                body->release();
            }
        }

        ImGui::InputFloat("Mass", &body->m_mass);
        ImGui::InputFloat("Friction", &body->m_friction.x);
        ImGui::InputFloat("Rolling friction", &body->m_friction.y);
        ImGui::InputFloat("Restitution", &body->m_restitution);
    }

    std::string m_meshPath;
};
