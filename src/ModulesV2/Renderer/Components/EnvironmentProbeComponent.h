#pragma once

#include "../../../CoreV2/API.h"

class EnvironmentProbeComponent : public Component {
public:
    EnvironmentProbeComponent();

    void serialize(nlohmann::json &j) override {
    }

    void deserialize(const nlohmann::json &j) override {
    }

    void registerWithSystems(EntityContext &ctx) override {
    }
};
