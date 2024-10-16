#pragma once

#include "../../../Core/API.h"

class DoorActivationBehaviour : public EntityBehaviour {
TYPE_DEFINITION(DoorActivationBehaviour);
public:
    DoorActivationBehaviour();

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    std::string getDescription() override;

    std::string getDisplayName() override {
        return "Character picking";
    }
};
