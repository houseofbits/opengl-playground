#include "MaterialEditWindowUI.h"

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../Systems/EditorUISystem.h"
#include "FileDialogHelper.h"
#include "TexturePreviewHelper.h"

MaterialEditWindowUI::MaterialEditWindowUI(EditorUISystem *editor) : m_EditorUISystem(editor),
                                                                     m_material() {
}

void MaterialEditWindowUI::process() {
    if (m_material.isValid()) {
        std::string title = "Edit material: " + m_material().m_Path;

        ImGui::SetNextWindowSize(ImVec2(450, 600));
        if (ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse)) {
            ImGui::ColorEdit3("Diffuse color##MATERIAL_COLOR", (float *) &m_material().m_DiffuseColor);
            ImGui::Dummy(ImVec2(0.0f, 10.0f));

            ImGui::Text("Diffuse texture");
            processTexture("DiffuseTexturePicker", m_material().m_Diffuse);
            ImGui::Dummy(ImVec2(0.0f, 10.0f));

            ImGui::Text("Normals texture");
            processTexture("NormalsTexturePicker", m_material().m_Normal);
            ImGui::Dummy(ImVec2(0.0f, 10.0f));

            ImGui::Text("Roughness texture");
            processTexture("RoughnessTexturePicker", m_material().m_Roughness);

            ImGui::Dummy(ImVec2(0.0f, 10.0f));

            ImGui::SliderFloat("Self illumination", &m_material().m_selfIllumination, 0.0f, 1.0f, "%.3f");

            ImGui::Checkbox("Does cast shadows", &m_material().m_doesCastShadows);
            ImGui::Checkbox("Does receive shadows", &m_material().m_doesReceiveShadows);

            ImGui::Dummy(ImVec2(0.0f, 20.0f));

            ImGui::Separator();

            if (ImGui::Button("Close")) {
                m_material.invalidate();
            }
            ImGui::SameLine();
            if (ImGui::Button("Save")) {
                m_material().write();
                m_material.invalidate();
            }

            ImGui::End();
        }
    }
}

void MaterialEditWindowUI::processTexture(std::string name, ResourceHandle<TextureResource> &handle) const {
    if (handle.isValid()) {
        ImGui::Image((void *) (intptr_t) handle().m_textureId, ImVec2(200, 200));

        if (ImGui::Button("Delete")) {
            handle.invalidate();
        }
        ImGui::SameLine();
    }
    if (FileInput(name, "Choose image file", ".png,.tga", "Path", handle().m_Path)) {
        m_EditorUISystem->m_ResourceManager->request(handle, handle().m_Path);
    }
}

void MaterialEditWindowUI::open(ResourceHandle<MaterialResource> &handle) {
    m_material.from(handle);
}