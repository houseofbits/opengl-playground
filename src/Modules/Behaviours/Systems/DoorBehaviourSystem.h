#pragma once

#include "../../../Core/API.h"
#include "../../Physics/Events/PhysicsPickingEvent.h"
#include "../../Physics/Events/PhysicsTriggerShapeEvent.h"
#include "../Components/DoorBehaviourComponent.h"
#include "../../Physics/Components/PhysicsSliderJointComponent.h"
#include "../../Physics/Components/PhysicsHingeJointComponent.h"

class DoorBehaviourSystem : public EntitySystem {
public:
    DoorBehaviourSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleCharacterPickingEvent(const PhysicsPickingEvent &);

    void handlePhysicsTriggerShapeEvent(const PhysicsTriggerShapeEvent &);

private:
    static void processJointBehaviour(BasePhysicsJoint *, DoorBehaviourComponent *);

    static void handleActivation(BasePhysicsJoint *, DoorBehaviourComponent *);

    EntityRelatedComponentRegistry<DoorBehaviourComponent, PhysicsSliderJointComponent> *m_sliderJointComponentRegistry;
    EntityRelatedComponentRegistry<DoorBehaviourComponent, PhysicsHingeJointComponent> *m_hingeJointComponentRegistry;
};
