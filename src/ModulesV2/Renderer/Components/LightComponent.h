#pragma once

#include "../../../CoreV2/API.h"

class LightComponent : public Component {
public:
    LightComponent();

    void serialize(nlohmann::json &j) override {
    }

    void deserialize(const nlohmann::json &j) override {
    }

    void registerWithSystems(EntityContext &ctx) override {
    }
};
