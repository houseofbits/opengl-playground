#pragma once

#include "../../../Core/API.h"

class CharacterPickingBehaviour : public EntityBehaviour {
TYPE_DEFINITION(CharacterPickingBehaviour);

public:
    CharacterPickingBehaviour();

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    std::string getDescription() override;

    void handleInputEvent(const InputEvent &);

    std::string getDisplayName() override {
        return "Character picking";
    }
};
