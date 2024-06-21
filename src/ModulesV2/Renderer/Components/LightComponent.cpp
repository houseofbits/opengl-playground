#include "LightComponent.h"
#include "../Systems/RendererSystem.h"

LightComponent::LightComponent() : Component(),
                                   m_TypeNameMap(),
                                   m_Type(SPOT),
                                   m_Color(1.0),
                                   m_Intensity(1.0),
                                   m_beamAngle(90.0),
                                   m_Attenuation(1.0) {
    m_TypeNameMap[Type::OMNI] = "OMNI";
    m_TypeNameMap[Type::SPOT] = "SPOT";
    m_TypeNameMap[Type::DIRECT] = "DIRECT";
}

void LightComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = getTypeName();
    j[COLOR_KEY] = m_Color;
    j[INTENSITY_KEY] = m_Intensity;
    j[BEAM_ANGLE_KEY] = m_beamAngle;
    j[ATTENUATION_KEY] = m_Attenuation;
}

void LightComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_Type = getTypeFromName(j.value(TYPE_KEY, getTypeName()));
    m_Color = j.value(COLOR_KEY, m_Color);
    m_Intensity = j.value(INTENSITY_KEY, m_Intensity);
    m_beamAngle = j.value(BEAM_ANGLE_KEY, m_beamAngle);
    m_Attenuation = j.value(ATTENUATION_KEY, m_Attenuation);
}

void LightComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<RendererSystem>(this);
}

std::string LightComponent::getTypeName() {
    return m_TypeNameMap[m_Type];
}

LightComponent::Type LightComponent::getTypeFromName(const std::string &name) {
    for (const auto &[key, value]: m_TypeNameMap) {
        if (value == name) {
            return key;
        }
    }

    return SPOT;
}