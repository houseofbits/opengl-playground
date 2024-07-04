#include "LightComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/RendererSystem.h"

LightComponent::LightComponent() : Component(),
                                   m_TypeNameMap(),
                                   m_Type(SPOT),
                                   m_Color(1.0),
                                   m_Intensity(1.0),
                                   m_beamAngle(90.0),
                                   m_Attenuation(1.0),
                                   m_Radius(1),
                                   m_Projection() {
    m_TypeNameMap[Type::OMNI] = "OMNI";
    m_TypeNameMap[Type::SPOT] = "SPOT";
    m_TypeNameMap[Type::DIRECT] = "DIRECT";
}

void LightComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = getLightTypeName();
    j[COLOR_KEY] = m_Color;
    j[INTENSITY_KEY] = m_Intensity;
    j[ATTENUATION_KEY] = m_Attenuation;
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
    m_Intensity = j.value(INTENSITY_KEY, m_Intensity);
    m_Attenuation = j.value(ATTENUATION_KEY, m_Attenuation);

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

void LightComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
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