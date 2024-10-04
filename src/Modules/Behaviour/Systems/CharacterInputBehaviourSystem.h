#pragma once

#include "../../../Core/API.h"

class CharacterInputBehaviourSystem : public EntitySystem {
public:
    CharacterInputBehaviourSystem();

    void process() override;

    void initialize(ResourceManager *) override;

    void registerEventHandlers(EventManager *eventManager) override;

    bool handleInputEvent(InputEvent *event);
};
