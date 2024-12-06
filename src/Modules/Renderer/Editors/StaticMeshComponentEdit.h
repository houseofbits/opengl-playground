#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../Components/StaticMeshComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class StaticMeshComponentEdit : public BaseComponentEdit {
public:

    std::string getName() override {
        return "Render mesh";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *mesh = entity.getComponent<StaticMeshComponent>();
        if (mesh == nullptr) {
            return;
        }

        if (ImGui::BeginCombo("Render##RENDER_TYPE", mesh->m_TargetRenderNameMap[mesh->m_targetRenderer].c_str())) {
            for (const auto &renderType: mesh->m_TargetRenderNameMap) {
                if (ImGui::Selectable(renderType.second.c_str(), mesh->m_targetRenderer == renderType.first)) {
                    mesh->m_targetRenderer = renderType.first;
                }
            }
            ImGui::EndCombo();
        }

        if (FileInput("ChooseModelFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath, mesh->m_Mesh().m_Path)) {
            mesh->m_Mesh.invalidate();
            system.m_ResourceManager->request(mesh->m_Mesh, m_meshPath);
        }

        if (FileInput("ChooseMaterialFile", "Choose JSON Material file", ".json", "Material", m_materialPath, mesh->m_Material().m_Path)) {
            mesh->m_Material.invalidate();
            system.m_ResourceManager->request(mesh->m_Material, m_materialPath);
        }

        if (mesh->m_Material.isValid()) {
            if (ImGui::Button("Edit material")) {
                system.openMaterialEditor(mesh->m_Material);
            }
        }

        if (TextPromptHelper::textPrompt("New material", "New material", "Filename")) {
            system.m_ResourceManager->request(mesh->m_Material,
                                                         "data/materials/" + TextPromptHelper::m_InputValue);
        }
    }

    std::string m_meshPath;
    std::string m_materialPath;
};
