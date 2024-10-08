#pragma once

#include "../../../Core/API.h"
#include "../Events/PhysicsPickingEvent.h"
#include "../Resources/PhysicsResource.h"

class JointsProcessingSystem : public EntitySystem {
public:
    JointsProcessingSystem();

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void process(EventManager &) override;

    void handleCharacterPickingEvent(const PhysicsPickingEvent *);

    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
