#pragma once

#include "../../../Core/API.h"

class EntityLinkingEvent;
class EntityLinkedComponent;

class EntityLinkingSystem : public EntitySystem {
public:
    EntityLinkingSystem();

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleEntityLinkingEvent(const EntityLinkingEvent&);

    void registerEntityComponents(Entity &entity) override;

private:
    SingleComponentRegistry<EntityLinkedComponent>* m_registry;
};
