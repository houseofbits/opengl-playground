#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../Components/StaticMeshComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processStaticMeshComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<StaticMeshComponent *>(c);

    static std::string m_meshPath;
    static std::string m_materialPath;

    if (ImGui::BeginCombo("Render##RENDER_TYPE",
                          component->m_TargetRenderNameMap[component->m_targetRenderer].c_str())) {
        for (const auto &renderType: component->m_TargetRenderNameMap) {
            if (ImGui::Selectable(renderType.second.c_str(), component->m_targetRenderer == renderType.first)) {
                component->m_targetRenderer = renderType.first;
            }
        }
        ImGui::EndCombo();
    }

    if (FileInput("ChooseModelFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath,
                  component->m_Mesh().m_Path)) {
        component->m_Mesh.invalidate();
        system.m_ResourceManager->request(component->m_Mesh, m_meshPath);
    }

    if (FileInput("ChooseMaterialFile", "Choose JSON Material file", ".json", "Material", m_materialPath,
                  component->m_Material().m_Path)) {
        component->m_Material.invalidate();
        system.m_ResourceManager->request(component->m_Material, m_materialPath);
    }

    if (component->m_Material.isValid()) {
        if (ImGui::Button("Edit material")) {
            system.openMaterialEditor(component->m_Material);
        }
    }

    if (TextPromptHelper::textPrompt("New material", "New material", "Filename")) {
        system.m_ResourceManager->request(component->m_Material,
                                          "data/materials/" + TextPromptHelper::m_InputValue);
    }
}
