#pragma once

#include "../../../Core/API.h"
#include "../../Physics/Events/CharacterPickingEvent.h"

class DoorUpdateSystem : public EntitySystem  {
public:
    DoorUpdateSystem();

    void process() override;
    void initialize(ResourceManager*) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleCharacterPickingEvent(CharacterPickingEvent* event);
};
