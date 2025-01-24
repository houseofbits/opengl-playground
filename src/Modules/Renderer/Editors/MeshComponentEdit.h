#pragma once

#include "../Components/MeshComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class MeshComponentEdit : public BaseComponentEdit {
public:

    std::string getName() override {
        return "Composite render mesh";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *mesh = entity.getComponent<MeshComponent>();
        if (mesh == nullptr) {
            return;
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
