#pragma once

#include "../../../Core/API.h"
#include "../../Physics/Events/PhysicsPickingEvent.h"

class DoorBehaviourSystem : public EntitySystem {
public:
    DoorBehaviourSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleCharacterPickingEvent(const PhysicsPickingEvent &);
};
