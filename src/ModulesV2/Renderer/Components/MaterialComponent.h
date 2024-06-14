#pragma once

#include "../../../CoreV2/API.h"

class MaterialComponent : public Component {
public:
    MaterialComponent();

    void serialize(nlohmann::json &j) override {
    }

    void deserialize(const nlohmann::json &j) override {
    }

    void registerWithSystems(EntityContext &ctx) override {
    }
};
