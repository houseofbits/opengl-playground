#include "MaterialEditWindowUI.h"

#include "../../../Renderer/Material/MaterialConfigurationSerializer.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../Systems/EditorUISystem.h"
#include "../Helpers/FileDialogHelper.h"
#include "../Helpers/TexturePreviewHelper.h"

MaterialEditWindowUI::MaterialEditWindowUI(EditorUISystem *editor) : m_EditorUISystem(editor),
                                                                     m_selectedMaterial(nullptr),
                                                                     m_isNewMaterialSelected(false),
                                                                     m_newMaterialResource(),
                                                                     m_newMaterialName(),
                                                                     m_texturePaths() {
    m_texturePaths["DiffuseTexturePicker"] = "";
    m_texturePaths["NormalsTexturePicker"] = "";
    m_texturePaths["RoughnessTexturePicker"] = "";
    m_texturePaths["EmissiveTexturePicker"] = "";
}

void MaterialEditWindowUI::process() {
    if (ImGui::Begin("Material editor", nullptr, ImGuiWindowFlags_NoCollapse)) {
        if (ImGui::BeginListBox("##MATERIAL_LIST", ImVec2(150, -1))) {
            auto materials = m_EditorUISystem->m_ResourceManager->getAllOfType<MaterialResource>();
            for (auto material: materials) {
                std::string valid = "";
                if (!material->isReady()) {
                    valid = " *";
                }
                std::string materialName = material->m_Path + valid;
                if (ImGui::Selectable(materialName.c_str(), m_selectedMaterial == material)) {
                    m_selectedMaterial = material;
                    m_isNewMaterialSelected = false;
                }
            }

            ImGui::EndListBox();
        }

        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        if (m_selectedMaterial != nullptr) {
            ImGui::Text("Name: %s", m_selectedMaterial->m_Path.c_str());
            ImGui::SameLine();
        }

        if (m_selectedMaterial != nullptr) {
            if (ImGui::Button("Save")) {
                saveMaterial();
            }

            ImGui::SameLine();
        }

        if (!m_isNewMaterialSelected && ImGui::Button("New material")) {
            m_newMaterialName = "";
            m_selectedMaterial = nullptr;
            m_isNewMaterialSelected = true;
        }

        if (m_isNewMaterialSelected) {
            ImGui::InputText("Name", &m_newMaterialName);

            ImGui::SameLine();

            if (ImGui::Button("Create")) {
                createEmptyMaterial();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel")) {
                m_isNewMaterialSelected = false;
            }
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        if (m_selectedMaterial != nullptr) {
            ImGui::Dummy(ImVec2(0.0f, 10.0f));

            int i = 1;
            const ImGuiID child_id = ImGui::GetID((void *) (intptr_t) i);
            ImGui::BeginChild(child_id, ImVec2(-1, -1));

            processMaterial();

            ImGui::EndChild();
        }

        ImGui::EndGroup();

        ImGui::End();
    }
}

void MaterialEditWindowUI::processMaterial() {
    ImGui::ColorEdit3("Diffuse color##MATERIAL-DIFFUSE__COLOR",
                      (float *) &m_selectedMaterial->m_materialConfiguration.diffuseColor);
    ImGui::ColorEdit3("Emissive color##MATERIAL_EMISSIVE_COLOR",
                      (float *) &m_selectedMaterial->m_materialConfiguration.emissiveColor);

    ImGui::SliderFloat("Roughness",
                       &m_selectedMaterial->m_materialConfiguration.roughnessFactor, 0.0f, 1.0f,
                       "%.3f");

    ImGui::SliderFloat("Metallic",
                       &m_selectedMaterial->m_materialConfiguration.metallicFactor, 0.0f, 1.0f,
                       "%.3f");

    ImGui::SliderFloat("Self illumination",
                       &m_selectedMaterial->m_materialConfiguration.selfIllumination, 0.0f, 1.0f,
                       "%.3f");


    ImGui::Checkbox("Does cast shadows", &m_selectedMaterial->m_materialConfiguration.doesCastShadows);
    ImGui::Checkbox("Does receive shadows", &m_selectedMaterial->m_materialConfiguration.doesReceiveShadows);


    ImGui::Text("Diffuse texture");
    processTexture("DiffuseTexturePicker", m_selectedMaterial->m_Diffuse);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Normals texture");
    processTexture("NormalsTexturePicker", m_selectedMaterial->m_Normal);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Roughness texture");
    processTexture("RoughnessTexturePicker", m_selectedMaterial->m_Roughness);

    ImGui::Text("Emissive texture");
    processTexture("EmissiveTexturePicker", m_selectedMaterial->m_Emissive);
}

void MaterialEditWindowUI::processTexture(const std::string &name, ResourceHandle<TextureResource> &handle) const {
    if (handle.isValid()) {
        ImGui::Image((void *) (intptr_t) handle().m_textureId, ImVec2(200, 200));

        std::string buttonName = "Delete##DeleteButton" + name;
        if (ImGui::Button(buttonName.c_str())) {
            handle.invalidate();
        }
        ImGui::SameLine();
    }

    std::string value = m_texturePaths.at(name);
    if (FileInput(name, "Choose image file", ".png,.tga", "Path", value, handle().m_Path)) {
        handle.invalidate();
        m_EditorUISystem->m_ResourceManager->request(handle, value);
    }
}

void MaterialEditWindowUI::saveMaterial() const {
    m_selectedMaterial->m_materialConfiguration.diffuseTextureUri = m_selectedMaterial->m_Diffuse().m_Path;
    m_selectedMaterial->m_materialConfiguration.normalTextureUri = m_selectedMaterial->m_Normal().m_Path;
    m_selectedMaterial->m_materialConfiguration.roughnessMetallicTextureUri = m_selectedMaterial->m_Roughness().m_Path;
    m_selectedMaterial->m_materialConfiguration.emissiveTextureUri = m_selectedMaterial->m_Emissive().m_Path;

    nlohmann::basic_json json;
    MaterialConfigurationSerializer::serialize(m_selectedMaterial->m_materialConfiguration, json);
    Json::writeFile(m_selectedMaterial->getFullPath(), json);
}

void MaterialEditWindowUI::createEmptyMaterial() const {
    Json::writeFile(MaterialResource::getFullPath(m_newMaterialName), nlohmann::basic_json());
}
