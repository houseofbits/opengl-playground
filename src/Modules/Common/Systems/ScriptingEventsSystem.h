#pragma once

#include "../../../Core/API.h"
#include "../../Physics/Events/PhysicsPickingEvent.h"
#include "../../Physics/Events/PhysicsTriggerShapeEvent.h"

class ScriptingEventsSystem : public EntitySystem {
public:
    ScriptingEventsSystem();

    void registerEventHandlers(EventManager &) override;

    void handleCharacterPickingEvent(const PhysicsPickingEvent &);

    void handlePhysicsTriggerShapeEvent(const PhysicsTriggerShapeEvent &);
};