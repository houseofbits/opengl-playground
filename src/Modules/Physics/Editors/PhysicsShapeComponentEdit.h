#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/PhysicsShapeComponent.h"

inline void processPhysicsShapeComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    static std::map<PhysicsShapeComponent::Type, std::string> types = {
        {PhysicsShapeComponent::TYPE_UNDEFINED, "-"},
        {PhysicsShapeComponent::TYPE_BOX, "Box"},
        {PhysicsShapeComponent::TYPE_SPHERE, "Sphere"},
        {PhysicsShapeComponent::TYPE_MESH, "Mesh"},
    };

    const auto component = dynamic_cast<PhysicsShapeComponent *>(c);

    if (ImGui::BeginCombo("Type##PHYSICS_SHAPE_TYPE", types[component->m_type].c_str())) {
        for (const auto &[type, name]: types) {
            if (ImGui::Selectable(name.c_str(), component->m_type == type)) {
                component->m_type = type;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::InputFloat3("Position", &component->m_localPosition[0]);

    glm::vec3 euler = glm::eulerAngles(component->m_localRotation);
    euler = glm::degrees(euler);
    if (ImGui::InputFloat3("Rotation", &euler[0])) {
        const glm::vec3 eulerRadians = glm::radians(euler);
        component->m_localRotation = glm::quat(eulerRadians);
    }

    if (component->m_type == PhysicsShapeComponent::TYPE_BOX) {
        ImGui::InputFloat3("Size", &component->m_boxSize[0]);
    }

    if (component->m_type == PhysicsShapeComponent::TYPE_SPHERE) {
        ImGui::InputFloat("Radius", &component->m_sphereRadius);
    }

    if (component->m_type == PhysicsShapeComponent::TYPE_MESH) {
        static std::string meshPath;
        if (FileInput("ChooseRigidBodyMeshFile2", "Choose GLTF Model file", ".gltf", "Model", meshPath,
                      component->m_meshResource().m_Path)) {
            component->m_meshResource.invalidate();
            system.m_ResourceManager->request(component->m_meshResource, meshPath);
        }
    }
}
