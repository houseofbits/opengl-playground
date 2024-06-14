#pragma once

#include "../../../CoreV2/API.h"

class CameraComponent : public Component {
public:
    CameraComponent();

    void serialize(nlohmann::json &j) override {
    }

    void deserialize(const nlohmann::json &j) override {
    }

    void registerWithSystems(EntityContext &ctx) override {
    }
};
