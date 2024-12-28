#include "ApplicationSystem.h"
#include "../../../Core/Entity/EntitySerializer.h"
#include "../../../Core/Helper/Time.h"

ApplicationSystem::ApplicationSystem() : EntitySystem(), m_ResourceManager(nullptr), m_WindowResource() {

}

void ApplicationSystem::initialize(ResourceManager &resourceManager, EventManager &eventManager) {
    m_ResourceManager = &resourceManager;

    resourceManager.request(m_WindowResource, "window");

    m_WindowResource().create(eventManager);
}

void ApplicationSystem::process(EventManager &) {
    setTime();
}

void ApplicationSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &ApplicationSystem::handleEntityCreationEvent);
}

void ApplicationSystem::handleEntityCreationEvent(const EntityCreationEvent &event) {
    if (event.m_Type == EntityCreationEvent::CREATE) {
        auto e = m_EntityContext->createEntityFromTemplate(event.m_ConfigurationName, *m_ResourceManager);
        e->m_Name = event.m_name;
        nlohmann::basic_json json;
        EntitySerializer::deserialize(*e, json, *m_ResourceManager);
    }
    if (event.m_Type == EntityCreationEvent::CLONE) {
        auto existingEntity = m_EntityContext->getEntity(event.m_entityId);
        if (existingEntity != nullptr) {
            nlohmann::basic_json json;
            EntitySerializer::serialize(*existingEntity, json);
            auto e = m_EntityContext->createEntityFromJson(json);
            EntitySerializer::deserialize(*e, json, *m_ResourceManager);
            e->m_Name = existingEntity->m_Name + "-COPY";
        }
    }
    if (event.m_Type == EntityCreationEvent::REMOVE) {
        m_EntityContext->removeEntity(event.m_entityId);
    }
    if (event.m_Type == EntityCreationEvent::CREATE_COMPONENT) {
        m_EntityContext->createComponentInplace(event.m_entityId, event.m_name);
    }
    if (event.m_Type == EntityCreationEvent::REMOVE_COMPONENT) {
        m_EntityContext->removeComponent(event.m_entityId, event.m_name);
    }
}

void ApplicationSystem::setTime() {
    float currentTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;

    Time::frameTime = currentTime - Time::timestamp;
    Time::timestamp = currentTime;
}
