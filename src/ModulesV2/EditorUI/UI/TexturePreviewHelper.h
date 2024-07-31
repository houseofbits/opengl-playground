#pragma once
#include "../../../CoreV2/Resources/ResourceHandle.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../Renderer/Resources/TextureResource.h"

#include <string>

class TexturePreviewHelper {
    public:
        inline static bool m_isPreviewWindowOpen = false;
        inline static int m_previewTextureId = 0;
        inline static std::string m_texturePath = "";

        static void process() {
            std::string title = "Texture: " + m_texturePath;
            if (TexturePreviewHelper::m_isPreviewWindowOpen && ImGui::Begin(title.c_str(), &TexturePreviewHelper::m_isPreviewWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Image((void *) (intptr_t) TexturePreviewHelper::m_previewTextureId, ImVec2(400, 400));

                ImGui::End();
            }
        }

        static void texturePreview(ResourceHandle<TextureResource>& resource) {
            if(ImGui::Button("View texture")) {
                TexturePreviewHelper::m_previewTextureId = (int)resource().m_textureId;
                TexturePreviewHelper::m_texturePath = resource().m_Path;
                TexturePreviewHelper::m_isPreviewWindowOpen = true;
            }
        }


};