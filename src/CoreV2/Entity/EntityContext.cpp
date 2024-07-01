#include "EntityContext.h"
#include "../../Helper/Log.h"
#include "../Module/EntityModule.h"
#include "EntitySerializer.h"
#include <iostream>
#include <memory>

EntityContext::EntityContext() : m_ComponentFactory(), m_EntityConfiguration(m_ComponentFactory), m_Entities(), m_Systems() {
}

Entity::TEntityPtr EntityContext::addEntity() {
    auto e = std::make_shared<Entity>();
    e->m_Id = Identity::create(Identity::ENTITY);
    m_Entities.push_back(e);

    return e;
}

void EntityContext::deserializeEntityMap(nlohmann::json &j) {
    m_EntityConfiguration.deserialize(j);
}

Entity::TEntityPtr EntityContext::createEntity(const std::string &configurationName, ResourceManager & resourceManager) {

    Entity::TEntityPtr e = addEntity();
    m_EntityConfiguration.buildEntity(*e, configurationName, resourceManager);

    return e;
}

void EntityContext::removeEntity(int entityId) {
    Entity* e = getEntity(entityId);
    if (e != nullptr) {
        e->unregisterFromSystems(*this);

        for(const auto& entity: m_Entities) {
            if (e->m_Id.id() == entityId) {
                m_Entities.remove(entity);
                break;
            }
        }
    }
}

void EntityContext::serializeEntities(nlohmann::json &j) {
    for (const auto &e: m_Entities) {
        nlohmann::json entityJson;
        EntitySerializer::serialize(*e, entityJson);
        j.push_back(entityJson);
    }
}

void EntityContext::deserializeEntities(nlohmann::json &j, ResourceManager & resourceManager) {
    for (const auto &entityJson: j.items()) {
        if (!entityJson.value().contains("type")) {
            Log::error("EntityContext::createEntities: Json does not contain entity type");
            continue;
        }
        Entity::TEntityPtr entity = createEntity(entityJson.value().at("type"), resourceManager);
        EntitySerializer::deserialize(*entity, entityJson.value(), resourceManager);
    }
}

void EntityContext::unregisterComponentFromSystems(Component *component) {
    for (const auto &system: m_Systems) {
        system->unregisterComponent(component);
    }
}

void EntityContext::registerEntitiesWithSystems() {
    for (const auto &e: m_Entities) {
        if (e->m_Status == Entity::CREATED && e->isReadyToRegister()) {
            e->registerWithSystems(*this);
        }
    }
}

void EntityContext::initializeSystems(ResourceManager* resourceManager, EventManager *eventManager) {
    for (const auto &system: m_Systems) {
        system->m_EventManager = eventManager;
        system->initialize(resourceManager);
        system->registerEventHandlers(eventManager);
    }
}

void EntityContext::processSystems() {
    registerEntitiesWithSystems();

    for (const auto &system: m_Systems) {
        system->process();
    }
}
