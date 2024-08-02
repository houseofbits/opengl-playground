#pragma once

#include "../TexturePreviewHelper.h"
#include "BaseComponentEdit.h"
#include "../FileDialogHelper.h"

class LightComponentEdit : public BaseComponentEdit {
public:
    inline static const std::map<LightComponent::Type, std::string> LIGHT_TYPE_NAME_MAP = {
            {LightComponent::SPOT, "SPOT"},
            {LightComponent::OMNI, "OMNI"},
            {LightComponent::DIRECT, "DIRECT"}};

    inline static const std::map<int, std::string> SHADOW_MAP_RESOLUTION = {
            {64, "64x64"},
            {128, "128x128"},
            {256, "256x256"},
            {512, "512x512"},
            {1024, "1024x1024"},
            {2048, "2048x2048"},
    };

    explicit LightComponentEdit(EditorUISystem *uiSystem) : BaseComponentEdit(uiSystem) {
    }

    bool isEntityEditable(Identity::Type entityId) override {
        return m_EditorUISystem->m_EntityContext
                ->doesEntityHaveComponent<LightComponent>(entityId);
    }

    std::string getName() override {
        return "Light";
    }

    void process(Identity::Type entityId) override {
        auto *light = m_EditorUISystem->getComponent<LightComponent>(entityId);
        if (light == nullptr) {
            return;
        }

        ImGui::SeparatorText("Light");

        ImGui::Checkbox("Is enabled", &light->m_isEnabled);

        if (ImGui::BeginCombo("Type##LIGHT_TYPE", LIGHT_TYPE_NAME_MAP.at(light->m_Type).c_str())) {
            for (const auto &lightName: LIGHT_TYPE_NAME_MAP) {
                if (ImGui::Selectable(lightName.second.c_str(), light->m_Type == lightName.first)) {
                    light->m_Type = lightName.first;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::ColorEdit3("Color##LIGHT_COLOR", (float *) &light->m_Color);
        ImGui::InputFloat("Intensity##LIGHT_INTENSITY", &light->m_Intensity, 0.1f, 1.0f, "%.1f");
        ImGui::InputFloat("Attenuation##LIGHT_ATTENUATION", &light->m_Attenuation, 0.1f, 1.0f, "%.1f");
        if (light->m_Type == LightComponent::DIRECT) {
            ImGui::InputFloat("Radius##LIGHT_DIRECT_RADIUS", &light->m_Radius, 0.1f, 0.5f, "%.1f");
        } else if (light->m_Type == LightComponent::SPOT) {
            ImGui::InputFloat("Beam angle##LIGHT_BEAM_ANGLE", &light->m_beamAngle, 0.5f, 1.0f, "%.0f");
        }

        if (FileInput("ChooseLightProjectorFile", "Choose image file", ".png,.tga", "Projector", light->m_Projection().m_Path)) {
            m_EditorUISystem->m_ResourceManager->request(light->m_Projection, light->m_Projection().m_Path);
        }

        TexturePreviewHelper::texturePreview(light->m_Projection);

        ImGui::Checkbox("Cast shadows", &light->m_doesCastShadows);
        if (light->m_doesCastShadows) {
            if (ImGui::BeginCombo("Shadow size##SHADOW_RESOLUTION", SHADOW_MAP_RESOLUTION.at(light->m_shadowResolution).c_str())) {
                for (const auto &resolution: SHADOW_MAP_RESOLUTION) {
                    if (ImGui::Selectable(resolution.second.c_str(), light->m_shadowResolution == resolution.first)) {
                        light->resizeShadowMaps(resolution.first);
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::InputFloat("Bias", &light->m_shadowBias, 0.001f, 0.01f, "%.5f");

            // for (int i = 0; i < light->m_ShadowMaps.size(); ++i) {
            //     std::string buttonName = "Preview " + std::to_string(i + 1);
            //     if (ImGui::Button(buttonName.c_str())) {
            //         //                std::cout<<light->m_ShadowMaps[i]->get().m_Path<<std::endl;
            //         m_isPreviewWindowOpen = true;
            //         m_previewTextureId = light->m_ShadowMaps[i]->get().m_textureRenderTarget.textureId;
            //         m_previewTextureSize = light->m_ShadowMaps[i]->get().m_textureRenderTarget.width;
            //     }
            // }
        }
    }
};
