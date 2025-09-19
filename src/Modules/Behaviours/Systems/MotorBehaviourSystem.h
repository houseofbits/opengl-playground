#pragma once

#include "../../../Core/API.h"
#include "../../Application/Events/InputEvent.h"
#include "../../Physics/Components/PhysicsHingeJointComponent.h"
#include "../Components/MotorBehaviourComponent.h"

class MotorBehaviourSystem : public EntitySystem {
public:
    MotorBehaviourSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleInputEvent(const InputEvent &);

private:
    EntityUniqueComponentRegistry<PhysicsHingeJointComponent> *m_hingeJointComponentRegistry{};
    EntityUniqueComponentRegistry<MotorBehaviourComponent> *m_motorBehaviourComponentRegistry{};
};
