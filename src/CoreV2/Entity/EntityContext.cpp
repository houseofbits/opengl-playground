#include "EntityContext.h"
#include "../../Helper/Log.h"
#include "../Module/EntityModule.h"
#include "EntitySerializer.h"
#include <iostream>
#include <memory>

EntityContext::EntityContext() : m_ComponentFactory(), m_EntityConfiguration(m_ComponentFactory), m_Entities(), m_Systems() {
}

Entity::TEntityPtr EntityContext::addEntity() {

    unsigned int lastId = 0;
    if (!m_Entities.empty()) {
        lastId = m_Entities.back()->m_Id;
    }

    auto e = std::make_shared<Entity>();
    e->m_Id = lastId + 1;

    m_Entities.push_back(e);

    return e;
}

void EntityContext::deserializeEntityMap(nlohmann::json &j) {
    m_EntityConfiguration.deserialize(j);
}

Entity::TEntityPtr EntityContext::createEntity(const std::string &configurationName) {

    Entity::TEntityPtr e = addEntity();
    m_EntityConfiguration.buildEntity(*e, configurationName);

    return e;
}

void EntityContext::serializeEntities(nlohmann::json &j) {
    for (const auto &e: m_Entities) {
        nlohmann::json entityJson;
        EntitySerializer::serialize(*e, entityJson);
        j.push_back(entityJson);
    }
}

void EntityContext::deserializeEntities(nlohmann::json &j) {
    for (const auto &entityJson: j.items()) {
        if (!entityJson.value().contains("type")) {
            Log::error("EntityContext::createEntities: Json does not contain entity type");
            continue;
        }
        Entity::TEntityPtr entity = createEntity(entityJson.value().at("type"));
        EntitySerializer::deserialize(*entity, entityJson.value());
    }
}

void EntityContext::unregisterComponentFromSystems(Component *component) {
    for (const auto &system: m_Systems) {
        system->unregisterComponent(component);
    }
}

void EntityContext::registerEntitiesWithSystems() {
    for (const auto &e: m_Entities) {
        e->registerWithSystems(*this);
    }
}

void EntityContext::initializeSystems() {
    for (const auto &system: m_Systems) {
        system->initialize();
    }
}

void EntityContext::processSystems() {
    for (const auto &system: m_Systems) {
        system->process();
    }
}
