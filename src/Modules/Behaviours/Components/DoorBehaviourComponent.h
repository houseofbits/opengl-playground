#pragma once

#include "../../../Core/API.h"

class DoorBehaviourComponent : public Component  {
TYPE_DEFINITION(DoorBehaviourComponent);

public:
    DoorBehaviourComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
};
