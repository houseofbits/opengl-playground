#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../Components/StaticMeshComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"

class StaticMeshComponentEdit : public ComponentEdit<StaticMeshComponent> {
public:
    explicit StaticMeshComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Render mesh";
    }

    void processEditor() override {
        if (ImGui::BeginCombo("Render##RENDER_TYPE", m_component->m_TargetRenderNameMap[m_component->m_targetRenderer].c_str())) {
            for (const auto &renderType: m_component->m_TargetRenderNameMap) {
                if (ImGui::Selectable(renderType.second.c_str(), m_component->m_targetRenderer == renderType.first)) {
                    m_component->m_targetRenderer = renderType.first;
                }
            }
            ImGui::EndCombo();
        }

        if (FileInput("ChooseModelFile", "Choose GLTF Model file", ".gltf", "Model", m_meshPath, m_component->m_Mesh().m_Path)) {
            m_component->m_Mesh.invalidate();
            m_editorSystem->m_ResourceManager->request(m_component->m_Mesh, m_meshPath);
        }

        if (FileInput("ChooseMaterialFile", "Choose JSON Material file", ".json", "Material", m_materialPath, m_component->m_Material().m_Path)) {
            m_component->m_Material.invalidate();
            m_editorSystem->m_ResourceManager->request(m_component->m_Material, m_materialPath);
        }

        if (m_component->m_Material.isValid()) {
            if (ImGui::Button("Edit material")) {
                m_editorSystem->openMaterialEditor(m_component->m_Material);
            }
        }

        if (TextPromptHelper::textPrompt("New material", "New material", "Filename")) {
            m_editorSystem->m_ResourceManager->request(m_component->m_Material,
                                                         "data/materials/" + TextPromptHelper::m_InputValue);
        }
    }

    std::string m_meshPath;
    std::string m_materialPath;
};
