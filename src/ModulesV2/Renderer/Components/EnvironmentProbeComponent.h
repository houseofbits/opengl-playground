#pragma once

#include "../../../CoreV2/API.h"

class EnvironmentProbeComponent : public Component {
public:
    inline static const std::string DEBUG_COLOR_KEY = "debugColor";

    EnvironmentProbeComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;

    glm::vec3 m_DebugColor;

//    glm::vec3 m_ViewOffset; //View point offset relative to probe position;
};
