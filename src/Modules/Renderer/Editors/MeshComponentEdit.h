#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../Components/MeshComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processMeshComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<MeshComponent *>(c);

    static std::string m_meshPath;
    static std::string m_materialPath;

    if (FileInput("ChooseModelFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath,
                  component->m_Mesh().m_Path)) {
        component->m_Mesh.invalidate();
        system.m_ResourceManager->request(component->m_Mesh, m_meshPath);
    }

    ImGui::Checkbox("Override material", &component->m_shouldOverrideMaterial);
    if (component->m_shouldOverrideMaterial) {
        if (ImGui::InputText("Material name", &m_materialPath)) {
            if (m_materialPath.empty()) {
                component->m_Material.invalidate();
            } else {
                system.m_ResourceManager->request(component->m_Material, m_materialPath);
            }
        }
    }

    if (ImGui::TreeNode("Nodes")) {
        for (const auto &node: component->m_Mesh().m_modelConfig.nodes) {
            std::string material_name;
            if (node.materialIndex >= 0) {
                std::string isMaterialValid;
                if (!component->m_Mesh().m_materials[node.materialIndex].get().isReady()) {
                    isMaterialValid = " *";
                }
                material_name = " - " + component->m_Mesh().m_materials[node.materialIndex].get().m_Path +
                                isMaterialValid;
            }
            std::string line = node.name + material_name;

            ImGui::Text("%s", line.c_str());
        }
        ImGui::TreePop();
    }
}
