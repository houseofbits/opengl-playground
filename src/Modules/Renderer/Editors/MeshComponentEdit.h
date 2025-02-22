#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../Components/MeshComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/Helpers/TextPromptHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"

class MeshComponentEdit : public ComponentEdit<MeshComponent> {
public:
    explicit MeshComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

    std::string getName() override {
        return "Composite render mesh";
    }

    void processEditor() override {
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
