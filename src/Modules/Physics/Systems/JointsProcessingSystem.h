#pragma once

#include "../../../Core/API.h"
#include "../Events/CharacterPickingEvent.h"
#include "../Resources/PhysicsResource.h"

class JointsProcessingSystem : public EntitySystem {
public:
    JointsProcessingSystem();

    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    void process() override;
    bool handleCharacterPickingEvent(CharacterPickingEvent* event);

    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
