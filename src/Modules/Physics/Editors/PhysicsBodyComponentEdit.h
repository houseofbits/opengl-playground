#pragma once

#include "./PhysicsBodyComponentEdit.h"
#include "../../../Core/API.h"
#include "../Components/PhysicsBodyComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Renderer/Components/StaticMeshComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"

inline void processPhysicsBodyComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto physicsBody = dynamic_cast<PhysicsBodyComponent *>(c);

    static std::map<PhysicsBodyComponent::BodyType, std::string> BodyTypeNameMap = {
        {PhysicsBodyComponent::BODY_TYPE_STATIC, "Static"},
        {PhysicsBodyComponent::BODY_TYPE_DYNAMIC, "Dynamic"},
    };

    static std::map<PhysicsBodyComponent::MeshType, std::string> MeshTypeNameMap = {
        {PhysicsBodyComponent::MESH_TYPE_CONVEX, "Convex mesh"},
        {PhysicsBodyComponent::MESH_TYPE_TRIANGLE, "Triangle mesh"},
    };

    if (ImGui::BeginCombo("Body type##PHYSICS_BODY_TYPE", BodyTypeNameMap[physicsBody->m_BodyType].c_str())) {
        for (const auto &bodyType: BodyTypeNameMap) {
            if (ImGui::Selectable(bodyType.second.c_str(), physicsBody->m_BodyType == bodyType.first)) {
                physicsBody->m_BodyType = bodyType.first;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Mesh type##COLLISION_MESH_TYPE", MeshTypeNameMap[physicsBody->m_MeshType].c_str())) {
        for (const auto &meshType: MeshTypeNameMap) {
            if (ImGui::Selectable(meshType.second.c_str(), physicsBody->m_MeshType == meshType.first)) {
                physicsBody->m_MeshType = meshType.first;
            }
        }
        ImGui::EndCombo();
    }

    static std::string meshPath;
    if (FileInput("ChooseRigidBodyMeshFile", "Choose GLTF Model file", ".gltf", "Model", meshPath,
                  physicsBody->m_meshResource().m_Path)) {
        physicsBody->m_meshResource.invalidate();
        system.m_ResourceManager->request(physicsBody->m_meshResource, meshPath);
    }

    if (ImGui::Button("Create from render mesh")) {
        if (auto *mesh = e->getComponent<StaticMeshComponent>(); mesh && mesh->m_Mesh.isReady()) {
            physicsBody->m_meshResource.invalidate();
            system.m_ResourceManager->request(physicsBody->m_meshResource, mesh->m_Mesh.get().m_Path);
        }
    }

    ImGui::InputFloat("Mass", &physicsBody->m_mass);
    ImGui::InputFloat("Friction", &physicsBody->m_friction.x);
    ImGui::InputFloat("Rolling friction", &physicsBody->m_friction.y);
    ImGui::InputFloat("Restitution", &physicsBody->m_restitution);
}