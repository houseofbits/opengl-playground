#pragma once

#include "../../../Core/API.h"

class GunBehaviourComponent : public Component {
    TYPE_DEFINITION(GunBehaviourComponent);

    inline static const std::string ACTIVE_KEY = "active";

public:
    GunBehaviourComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    bool m_isActive;
};