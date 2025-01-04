#pragma once

#include "../Components/MeshComponent.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../../Editor/UI/BaseComponentEdit.h"

class MeshComponentEdit : public BaseComponentEdit {
public:

    std::string getName() override {
        return "Render mesh";
    }

    void process(Entity &entity, EditorUISystem& system) override {
        auto *mesh = entity.getComponent<MeshComponent>();
        if (mesh == nullptr) {
            return;
        }

        if (FileInput("ChooseModelFile", "Choose FBX Model file", ".FBX", "Model", m_meshPath, mesh->m_Mesh().m_Path)) {
            mesh->m_Mesh.invalidate();
            system.m_ResourceManager->request(mesh->m_Mesh, m_meshPath);
        }
    }

    std::string m_meshPath;
    std::string m_materialPath;
};
