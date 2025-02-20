#include "PhysicsBodyComponentEdit.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Renderer/Components/StaticMeshComponent.h"
#include "../Components/PhysicsBodyComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"

void PhysicsBodyComponentEdit::processEditor() {
    static std::map<PhysicsBodyComponent::BodyType, std::string> BodyTypeNameMap = {
        {PhysicsBodyComponent::BODY_TYPE_STATIC, "Static"},
        {PhysicsBodyComponent::BODY_TYPE_DYNAMIC, "Dynamic"},
    };

    static std::map<PhysicsBodyComponent::MeshType, std::string> MeshTypeNameMap = {
        {PhysicsBodyComponent::MESH_TYPE_CONVEX, "Convex mesh"},
        {PhysicsBodyComponent::MESH_TYPE_TRIANGLE, "Triangle mesh"},
    };

    if (m_component == nullptr) {
        return;
    }

    if (ImGui::BeginCombo("Body type##PHYSICS_BODY_TYPE", BodyTypeNameMap[m_component->m_BodyType].c_str())) {
        for (const auto &bodyType: BodyTypeNameMap) {
            if (ImGui::Selectable(bodyType.second.c_str(), m_component->m_BodyType == bodyType.first)) {
                m_component->m_BodyType = bodyType.first;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Mesh type##COLLISION_MESH_TYPE", MeshTypeNameMap[m_component->m_MeshType].c_str())) {
        for (const auto &meshType: MeshTypeNameMap) {
            if (ImGui::Selectable(meshType.second.c_str(), m_component->m_MeshType == meshType.first)) {
                m_component->m_MeshType = meshType.first;
            }
        }
        ImGui::EndCombo();
    }

    if (FileInput("ChooseRigidBodyMeshFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath,
                  m_component->m_meshResource().m_Path)) {
        m_component->m_meshResource.invalidate();
        m_editorSystem->m_ResourceManager->request(m_component->m_meshResource, m_meshPath);
    }

    if (ImGui::Button("Create from render mesh")) {
        auto *mesh = m_entity->getComponent<StaticMeshComponent>();
        if (mesh && mesh->m_Mesh.isReady()) {
            m_component->m_meshResource.invalidate();
            m_editorSystem->m_ResourceManager->request(m_component->m_meshResource, mesh->m_Mesh.get().m_Path);
        }
    }

    ImGui::InputFloat("Mass", &m_component->m_mass);
    ImGui::InputFloat("Friction", &m_component->m_friction.x);
    ImGui::InputFloat("Rolling friction", &m_component->m_friction.y);
    ImGui::InputFloat("Restitution", &m_component->m_restitution);
}
