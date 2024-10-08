#pragma once

#include "../../../Core/API.h"

class CharacterMouseLookBehaviour : public EntityBehaviour {
TYPE_DEFINITION(CharacterMouseLookBehaviour);

public:
    CharacterMouseLookBehaviour();

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    std::string getDescription() override;

    void handleInputEvent(const InputEvent *);

    std::string getDisplayName() override {
        return "Character mouse-look";
    }
};
