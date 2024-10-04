#pragma once

#include "../../../Core/API.h"
#include "../Events/PhysicsPickingEvent.h"
#include "../Resources/PhysicsResource.h"

class JointsProcessingSystem : public EntitySystem {
public:
    JointsProcessingSystem();

    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    void process() override;
    bool handleCharacterPickingEvent(PhysicsPickingEvent* event);

    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
