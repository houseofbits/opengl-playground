#pragma once

#include "../../Editor/Helpers/TexturePreviewHelper.h"
#include "../../Editor/Helpers/FileDialogHelper.h"
#include "../Components/LightComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"

inline void processLightComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    static const std::map<LightComponent::Type, std::string> LIGHT_TYPE_NAME_MAP = {
        {LightComponent::SPOT, "SPOT"},
        {LightComponent::OMNI, "OMNI"},
        {LightComponent::DIRECT, "DIRECT"}
    };

    static const std::map<int, std::string> SHADOW_MAP_RESOLUTION = {
        {64, "64x64"},
        {128, "128x128"},
        {256, "256x256"},
        {512, "512x512"},
        {1024, "1024x1024"},
        {2048, "2048x2048"},
    };

    auto component = dynamic_cast<LightComponent *>(c);

    static std::string m_projectorPath;

    ImGui::Checkbox("Is enabled", &component->m_isEnabled);

    if (ImGui::BeginCombo("Type##LIGHT_TYPE", LIGHT_TYPE_NAME_MAP.at(component->m_Type).c_str())) {
        for (const auto &lightName: LIGHT_TYPE_NAME_MAP) {
            if (ImGui::Selectable(lightName.second.c_str(), component->m_Type == lightName.first)) {
                component->m_Type = lightName.first;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::ColorEdit3("Color##LIGHT_COLOR", reinterpret_cast<float *>(&component->m_Color));
    ImGui::InputFloat("Intensity##LIGHT_INTENSITY", &component->m_Intensity, 0.1f, 1.0f, "%.1f");
    ImGui::InputFloat("Attenuation##LIGHT_ATTENUATION", &component->m_Attenuation, 0.1f, 1.0f, "%.1f");
    if (component->m_Type == LightComponent::DIRECT) {
        ImGui::InputFloat("Radius##LIGHT_DIRECT_RADIUS", &component->m_Radius, 0.1f, 0.5f, "%.1f");
    } else if (component->m_Type == LightComponent::SPOT) {
        ImGui::InputFloat("Beam angle##LIGHT_BEAM_ANGLE", &component->m_beamAngle, 0.5f, 1.0f, "%.0f");
    }

    if (FileInput("ChooseLightProjectorFile", "Choose image file", ".png,.tga", "Projector", m_projectorPath,
                  component->m_Projection().m_Path)) {
        component->m_Projection.invalidate();
        system.m_ResourceManager->request(component->m_Projection, m_projectorPath);
    }

    TexturePreviewHelper::texturePreview(component->m_Projection);

    ImGui::Checkbox("Cast shadows", &component->m_doesCastShadows);
    if (component->m_doesCastShadows) {
        if (ImGui::BeginCombo("Shadow size##SHADOW_RESOLUTION",
                              SHADOW_MAP_RESOLUTION.at(component->m_shadowResolution).c_str())) {
            for (const auto &resolution: SHADOW_MAP_RESOLUTION) {
                if (ImGui::Selectable(resolution.second.c_str(), component->m_shadowResolution == resolution.first)) {
                    component->resizeShadowMaps(resolution.first);
                }
            }
            ImGui::EndCombo();
        }

        ImGui::DragFloat("Bias", &component->m_shadowBias, 0.0000001f, 0.0f, 0.01f, "%.5f");
        ImGui::DragFloat("Blur radius", &component->m_blurRadius, 0.01f, 0.0f, 20.0f, "%.5f");

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
