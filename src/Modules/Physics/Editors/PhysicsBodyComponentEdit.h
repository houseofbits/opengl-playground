#pragma once

#include "./PhysicsBodyComponentEdit.h"
#include "../../../Core/API.h"
#include "../Components/PhysicsBodyComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Renderer/Components/StaticMeshComponent.h"

inline void processPhysicsBodyComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto physicsBody = dynamic_cast<PhysicsBodyComponent *>(c);

    static std::map<PhysicsBodyComponent::BodyType, std::string> BodyTypeNameMap = {
        {PhysicsBodyComponent::BODY_TYPE_STATIC, "Static"},
        {PhysicsBodyComponent::BODY_TYPE_DYNAMIC, "Dynamic"},
    };

    if (ImGui::BeginCombo("Body type##PHYSICS_BODY_TYPE", BodyTypeNameMap[physicsBody->m_BodyType].c_str())) {
        for (const auto &bodyType: BodyTypeNameMap) {
            if (ImGui::Selectable(bodyType.second.c_str(), physicsBody->m_BodyType == bodyType.first)) {
                physicsBody->m_BodyType = bodyType.first;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Is sensor", &physicsBody->m_isSensor);
    if (physicsBody->m_isSensor) {
        ImGui::Checkbox("Disable actions", &physicsBody->m_excludeSensorFromActionHit);
    }
    ImGui::SeparatorText("Physics properties");
    ImGui::InputFloat("Gravity factor", &physicsBody->m_gravityFactor);
    ImGui::InputFloat("Mass", &physicsBody->m_mass);
    ImGui::InputFloat("Friction", &physicsBody->m_friction.x);
    ImGui::InputFloat("Rolling friction", &physicsBody->m_friction.y);
    ImGui::InputFloat("Restitution", &physicsBody->m_restitution);
    ImGui::InputFloat("Linear damping", &physicsBody->m_damping.x);
    ImGui::InputFloat("Angular damping", &physicsBody->m_damping.y);
}