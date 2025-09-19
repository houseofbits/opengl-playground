#pragma once

#include "../../../Core/API.h"
#include "../Components/GunBehaviourComponent.h"
#include "../../Physics/Components/PhysicsSwingTwistJointComponent.h"
#include "../../Physics/Events/PhysicsPickingEvent.h"
#include "../../Physics/Events/PhysicsSensorEvent.h"
#include "../../Application/Events/InputEvent.h"

class GunBehaviourSystem : public EntitySystem {
public:
    GunBehaviourSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handlePhysicsPickingEvent(const PhysicsPickingEvent &);

    void handlePhysicsSensorEvent(const PhysicsSensorEvent &);

    void handleInputEvent(const InputEvent &);

private:
    EntityUniqueComponentRegistry<GunBehaviourComponent> *m_registry;

    Entity* findActiveGunEntity() const;
};
