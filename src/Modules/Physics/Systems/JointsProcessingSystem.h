#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsResource.h"
#include "../Helpers/BasePhysicsJoint.h"
#include "../Components/PhysicsHingeJointComponent.h"
#include "../Components/PhysicsFixedJointComponent.h"
#include "../Components/PhysicsSliderJointComponent.h"
#include "../../../Modules/Application/Events/SystemEvent.h"

class EntityLinkingEvent;

class JointsProcessingSystem : public EntitySystem {
public:
    JointsProcessingSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void process(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    ResourceHandle<PhysicsResource> m_PhysicsResource;

private:
    void releaseAll() const;

    SameComponentRegistry<BasePhysicsJoint> *m_physicsJoints;
    bool m_isSimulationDisabled;
};
