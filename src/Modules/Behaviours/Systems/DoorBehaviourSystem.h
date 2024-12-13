#pragma once

#include "../../../Core/API.h"
#include "../../Physics/Events/PhysicsPickingEvent.h"
#include "../Components/DoorBehaviourComponent.h"
#include "../../Physics/Components/PhysicsSliderJointComponent.h"
#include "../../Physics/Components/PhysicsHingeJointComponent.h"

class DoorBehaviourSystem : public EntitySystem {
public:
    DoorBehaviourSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleCharacterPickingEvent(const PhysicsPickingEvent &);

private:
    static void handleJointPickingEvent(BasePhysicsJoint *, DoorBehaviourComponent *);

    static void processJointBehaviour(BasePhysicsJoint *, DoorBehaviourComponent *);

    RelatedComponentRegistry<DoorBehaviourComponent, PhysicsSliderJointComponent> *m_sliderJointComponentRegistry;
    RelatedComponentRegistry<DoorBehaviourComponent, PhysicsHingeJointComponent> *m_hingeJointComponentRegistry;
};
