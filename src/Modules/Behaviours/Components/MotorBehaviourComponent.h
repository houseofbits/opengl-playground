#pragma once

#include "../../../Core/API.h"

class MotorBehaviourComponent : public Component {
    TYPE_DEFINITION(MotorBehaviourComponent);

    inline static const std::string SOME_KEY = "someKey";

public:
    MotorBehaviourComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    bool m_motorOn{false};
};