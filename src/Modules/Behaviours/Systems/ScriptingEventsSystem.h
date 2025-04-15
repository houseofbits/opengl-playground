#pragma once

#include "../../../Core/API.h"
#include "../../Physics/Events/PhysicsPickingEvent.h"
#include "../../Physics/Events/PhysicsSensorEvent.h"
#include "../../Application/Events/InputEvent.h"
#include "../../Application/Events/SystemEvent.h"
#include "../Components/ScriptingComponent.h"

class ScriptingEventsSystem final : public EntitySystem {
public:
    ScriptingEventsSystem();

    void registerEventHandlers(EventManager &) override;

    void handlePhysicsPickingEvent(const PhysicsPickingEvent &);

    void handlePhysicsSensorEvent(const PhysicsSensorEvent &);

    void handleInputEvent(const InputEvent &);

    void handleSystemEvent(const SystemEvent &event);

private:
    void runAllScripts() const;

    bool m_isSimulationDisabled{true};
    SameComponentRegistry<ScriptingComponent> *m_registry;
};
