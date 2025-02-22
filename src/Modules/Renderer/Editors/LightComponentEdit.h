#pragma once

#include "../../Editor/Helpers/TexturePreviewHelper.h"
#include "../../Editor/UI/ComponentEdit.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../Components/LightComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"

class LightComponentEdit : public ComponentEdit<LightComponent> {
public:
    explicit LightComponentEdit(EditorUISystem &editorSystem) : ComponentEdit(editorSystem) {
    }

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

    std::string getName() override {
        return "Light";
    }

    void processEditor() override {
        ImGui::Checkbox("Is enabled", &m_component->m_isEnabled);

        if (ImGui::BeginCombo("Type##LIGHT_TYPE", LIGHT_TYPE_NAME_MAP.at(m_component->m_Type).c_str())) {
            for (const auto &lightName: LIGHT_TYPE_NAME_MAP) {
                if (ImGui::Selectable(lightName.second.c_str(), m_component->m_Type == lightName.first)) {
                    m_component->m_Type = lightName.first;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::ColorEdit3("Color##LIGHT_COLOR", reinterpret_cast<float *>(&m_component->m_Color));
        ImGui::InputFloat("Intensity##LIGHT_INTENSITY", &m_component->m_Intensity, 0.1f, 1.0f, "%.1f");
        ImGui::InputFloat("Attenuation##LIGHT_ATTENUATION", &m_component->m_Attenuation, 0.1f, 1.0f, "%.1f");
        if (m_component->m_Type == LightComponent::DIRECT) {
            ImGui::InputFloat("Radius##LIGHT_DIRECT_RADIUS", &m_component->m_Radius, 0.1f, 0.5f, "%.1f");
        } else if (m_component->m_Type == LightComponent::SPOT) {
            ImGui::InputFloat("Beam angle##LIGHT_BEAM_ANGLE", &m_component->m_beamAngle, 0.5f, 1.0f, "%.0f");
        }

        if (FileInput("ChooseLightProjectorFile", "Choose image file", ".png,.tga", "Projector", m_projectorPath, m_component->m_Projection().m_Path)) {
            m_component->m_Projection.invalidate();
            m_editorSystem->m_ResourceManager->request(m_component->m_Projection, m_projectorPath);
        }

        TexturePreviewHelper::texturePreview(m_component->m_Projection);

        ImGui::Checkbox("Cast shadows", &m_component->m_doesCastShadows);
        if (m_component->m_doesCastShadows) {
            if (ImGui::BeginCombo("Shadow size##SHADOW_RESOLUTION", SHADOW_MAP_RESOLUTION.at(m_component->m_shadowResolution).c_str())) {
                for (const auto &resolution: SHADOW_MAP_RESOLUTION) {
                    if (ImGui::Selectable(resolution.second.c_str(), m_component->m_shadowResolution == resolution.first)) {
                        m_component->resizeShadowMaps(resolution.first);
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::InputFloat("Bias", &m_component->m_shadowBias, 0.001f, 0.01f, "%.5f");

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

    std::string m_projectorPath;
};
