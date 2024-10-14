#pragma once

#include "../../../Core/API.h"

class CharacterControlBehaviour : public EntityBehaviour {
TYPE_DEFINITION(CharacterControlBehaviour);

public:
    CharacterControlBehaviour();

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    std::string getDescription() override;

    void handleInputEvent(const InputEvent &);

    std::string getDisplayName() override {
        return "Character controller";
    }
};
