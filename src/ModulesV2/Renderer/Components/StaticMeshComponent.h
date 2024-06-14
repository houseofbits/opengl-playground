#pragma once

#include "../../../CoreV2/API.h"

class StaticMeshComponent : public Component {
public:
    StaticMeshComponent();

    void serialize(nlohmann::json &j) override {
        j["model"] = modelFileName;
    }

    void deserialize(const nlohmann::json &j) override {
        j.at("model").get_to(modelFileName);
    }

    void registerWithSystems(EntityContext& ctx) override;
    std::string modelFileName;
};
