#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsResource.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "../Components/PhysicsHingeJointComponent.h"
#include "../Components/PhysicsFixedJointComponent.h"
#include "../Components/PhysicsSliderJointComponent.h"

class EntityLinkingEvent;

class JointsProcessingSystem : public EntitySystem {
public:
    JointsProcessingSystem();

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void process(EventManager &) override;

    void handleEntityLinkingEvent(const EntityLinkingEvent&);

    ResourceHandle<PhysicsResource> m_PhysicsResource;

private:
    void processJoint(Identity::Type entityId, BasePhysicsJoint* joint) const;

    SingleComponentRegistry<PhysicsHingeJointComponent>* m_hingeJointComponentRegistry;
    SingleComponentRegistry<PhysicsFixedJointComponent>* m_fixedJointComponentRegistry;
    SingleComponentRegistry<PhysicsSliderJointComponent>* m_sliderJointComponentRegistry;
};
