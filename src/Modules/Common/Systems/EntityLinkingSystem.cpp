#include "EntityLinkingSystem.h"

#include "../Events/EntityLinkingEvent.h"
#include "../Helpers/EntityLinkedComponent.h"

EntityLinkingSystem::EntityLinkingSystem() {
    // m_registry = useEntityUniqueComponentRegistry<EntityLinkedComponent>();
}

void EntityLinkingSystem::process(EventManager &manager) {
    // for (const auto &[id, component]: m_registry->container()) {
    //     if (component->shouldResolveLinkedEntityId()) {
    //         auto entity = m_EntityContext->findEntity(component->getLinkedEntityName());
    //         if (entity) {
    //             component->setLinkedEntityId(entity->m_Id.id());
    //         }
    //     }
    // }
}

void EntityLinkingSystem::registerEventHandlers(EventManager &eventManager) {
    // eventManager.registerEventReceiver(this, &EntityLinkingSystem::handleEntityLinkingEvent);
    // //TODO: entity name change event
    // //TODO: entity removal event
}

void EntityLinkingSystem::handleEntityLinkingEvent(const EntityLinkingEvent &event) {
    // if (event.m_linkType == EntityLinkingEvent::TYPE_UNLINK) {
    //     // unregisterComponents(event.m_componentId);
    // }
    //
    // if (event.m_linkType == EntityLinkingEvent::TYPE_LINK_WITH_ID || event.m_linkType ==
    //     EntityLinkingEvent::TYPE_LINK_WITH_NAME) {
    //     auto component = m_registry->get(event.m_componentId);
    //     if (!component) {
    //         component = m_EntityContext->getComponent<EntityLinkedComponent>(event.m_componentId);
    //         if (component) {
    //             m_registry->registerComponent(event.m_componentId, component);
    //         } else {
    //             return;
    //         }
    //     }
    //     if (event.m_linkType == EntityLinkingEvent::TYPE_LINK_WITH_ID) {
    //         if (const auto entity = m_EntityContext->getEntity(event.m_linkedEntityId)) {
    //             component->setLinkedEntityName(entity->m_Name);
    //         }
    //         component->setLinkedEntityId(event.m_linkedEntityId);
    //     }
    //     if (event.m_linkType == EntityLinkingEvent::TYPE_LINK_WITH_NAME) {
    //         component->setLinkedEntityName(event.m_linkedEntityName);
    //     }
    // }
}

void EntityLinkingSystem::registerEntityComponents(Entity &entity) {
    // for (const auto &c: entity.m_Components) {
    //     if (auto cl = dynamic_cast<EntityLinkedComponent *>(c.get()); cl && cl->isLinkableToEntity()) {
    //         m_registry->registerComponent(c->m_Id.id(), cl);
    //     }
    // }
}
