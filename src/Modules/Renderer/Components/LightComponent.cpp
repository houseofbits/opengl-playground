#include "LightComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"

LightComponent::LightComponent() : Component(),
                                   m_isEnabled(true),
                                   m_TypeNameMap(),
                                   m_Type(SPOT),
                                   m_Color(1.0),
                                   m_Intensity(1.0),
                                   m_beamAngle(90.0),
                                   m_Attenuation(1.0),
                                   m_Radius(1),
                                   m_ShadowMaps(),
                                   m_Projection(),
                                   m_doesCastShadows(false),
                                   m_shadowResolution(512),
                                   m_shadowBias(0),
                                   m_lightBufferIndices() {
    m_TypeNameMap[Type::OMNI] = "OMNI";
    m_TypeNameMap[Type::SPOT] = "SPOT";
    m_TypeNameMap[Type::DIRECT] = "DIRECT";
}

void LightComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = getLightTypeName();
    j[COLOR_KEY] = m_Color;
    j[ENABLED_KEY] = m_isEnabled;
    j[INTENSITY_KEY] = m_Intensity;
    j[ATTENUATION_KEY] = m_Attenuation;
    j[CAST_SHADOWS_KEY] = m_doesCastShadows;
    if (m_doesCastShadows) {
        j[SHADOW_RESOLUTION_KEY] = m_shadowResolution;
        j[SHADOW_BIAS_KEY] = m_shadowBias;
    }
    if (m_Projection.isValid()) {
        j[PROJECTION_TEXTURE_KEY] = m_Projection().m_Path;
    }
    if (m_Type == DIRECT) {
        j[RADIUS_KEY] = m_Radius;
    } else if (m_Type == SPOT) {
        j[BEAM_ANGLE_KEY] = m_beamAngle;
    }
}

void LightComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_Type = getLightTypeFromName(j.value(TYPE_KEY, getLightTypeName()));
    m_Color = j.value(COLOR_KEY, m_Color);
    m_isEnabled = j.value(ENABLED_KEY, m_isEnabled);
    m_Intensity = j.value(INTENSITY_KEY, m_Intensity);
    m_Attenuation = j.value(ATTENUATION_KEY, m_Attenuation);
    m_doesCastShadows = j.value(CAST_SHADOWS_KEY, m_doesCastShadows);
    m_shadowResolution = j.value(SHADOW_RESOLUTION_KEY, m_shadowResolution);
    m_shadowBias = j.value(SHADOW_BIAS_KEY, m_shadowBias);

    if (j.contains(PROJECTION_TEXTURE_KEY)) {
        std::string filename = j.value(PROJECTION_TEXTURE_KEY, m_Projection().m_Path);
        resourceManager.request(m_Projection, filename);
    }

    if (m_Type == DIRECT) {
        m_Radius = j.value(RADIUS_KEY, m_Radius);
    } else if (m_Type == SPOT) {
        m_beamAngle = j.value(BEAM_ANGLE_KEY, m_beamAngle);
    }
}

std::string LightComponent::getLightTypeName() {
    return m_TypeNameMap[m_Type];
}

LightComponent::Type LightComponent::getLightTypeFromName(const std::string &name) {
    for (const auto &[key, value]: m_TypeNameMap) {
        if (value == name) {
            return key;
        }
    }

    return SPOT;
}

void LightComponent::removeShadowMaps() {
    for (auto &resource: m_ShadowMaps) {
        resource->invalidate();
        delete resource;
    }
    m_ShadowMaps.clear();
}

void LightComponent::resizeShadowMaps(int size) {
    m_shadowResolution = size;
    for (auto &resource: m_ShadowMaps) {
        if (resource->isValid()) {
            resource->get().m_Resolution = m_shadowResolution;
            resource->get().resize();
        }
    }
}

std::string LightComponent::getShadowMapResourceName(int viewIndex) {
    return "ShadowMap_" + std::to_string(m_Id.id()) + "_" + std::to_string(viewIndex);
}

void LightComponent::prepareShadowMapResources(ResourceManager &resourceManager) {
    if (m_doesCastShadows) {
        //Remove unused resources
        while (m_ShadowMaps.size() > m_lightBufferIndices.size()) {
            m_ShadowMaps.back()->invalidate();
            delete m_ShadowMaps.back();
            m_ShadowMaps.pop_back();
        }

        //Add needed resources
        while (m_ShadowMaps.size() < m_lightBufferIndices.size()) {
            auto *p = new ResourceHandle<ShadowMapResource>();
            m_ShadowMaps.push_back(p);
            resourceManager.request(*m_ShadowMaps.back(), getShadowMapResourceName((int) m_ShadowMaps.size() - 1));

            if (m_ShadowMaps.back()->get().m_Resolution != m_shadowResolution) {
                m_ShadowMaps.back()->get().m_Resolution = m_shadowResolution;
                m_ShadowMaps.back()->get().resize();
            }
        }
    } else {
        removeShadowMaps();
    }
}

int LightComponent::getNumberOfBufferLights() const {
    if (m_Type == OMNI) {
        return 6;
    }

    return 1;
}
