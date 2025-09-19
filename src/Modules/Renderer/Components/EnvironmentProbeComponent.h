#pragma once

#include "../../../Core/API.h"

class EnvironmentProbeComponent : public Component {
TYPE_DEFINITION(EnvironmentProbeComponent);

public:
    inline static const std::string DEBUG_COLOR_KEY = "debugColor";

    EnvironmentProbeComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    std::string getListName(Entity *e) {
        return e->m_Name + " " + " PROBE";
    }

    glm::vec3 m_DebugColor;
    int m_cubeMapLayerIndex;

    //    glm::vec3 m_ViewOffset; //View point offset relative to probe position;
};
