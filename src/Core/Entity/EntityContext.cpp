#include "EntityContext.h"
#include "EntitySerializer.h"
#include <memory>

EntityContext::EntityContext() : m_ComponentFactory(),
                                 m_EntityConfiguration(m_ComponentFactory),
                                 m_Entities() {
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

Entity::TEntityPtr
EntityContext::createEntityFromTemplate(const std::string &configurationName, ResourceManager &resourceManager) {
    Entity::TEntityPtr e = addEntity();
    m_EntityConfiguration.buildEntity(*e, configurationName, resourceManager);

    return e;
}

std::shared_ptr<Entity>
EntityContext::createEntityFromJson(nlohmann::json &entityJson) {
    Entity::TEntityPtr e = addEntity();

    for (auto &[key, value]: entityJson.items()) {
        if (key == "type" || key == "name" || key == "behaviours") {
            continue;
        }

        const auto [type, name] = EntitySerializer::getComponentTypeAndNameFromNameKey(key);

        Component *c = m_ComponentFactory.createInstance(type);
        if (c == nullptr) {
            Log::error("Component class not found " + key);
            continue;
        }

        c->m_Id = Identity::create(Identity::COMPONENT);
        c->m_EntityId = e->m_Id;
        c->m_Status = Component::STATUS_CREATED;
        c->m_Name = name;

        e->addComponent(*c);
    }

    return e;
}

void EntityContext::removeEntity(int entityId) {
    Entity *e = getEntity(entityId);
    if (e != nullptr) {
        unregisterEntityFromSystems(*e);

        for (const auto &entity: m_Entities) {
            if (entity->m_Id.id() == entityId) {
                m_Entities.remove(entity);
                break;
            }
        }
    }
}

void EntityContext::serializeEntities(nlohmann::json &j) const {
    for (const auto &e: m_Entities) {
        nlohmann::json entityJson;
        EntitySerializer::serialize(*e, entityJson);
        j.push_back(entityJson);
    }
}

void EntityContext::deserializeEntities(nlohmann::json &j, ResourceManager &resourceManager) {
    for (const auto &entityJson: j.items()) {
        Entity::TEntityPtr entity = createEntityFromJson(entityJson.value());
        EntitySerializer::deserialize(*entity, entityJson.value(), resourceManager);
    }
}

void EntityContext::unregisterEntityFromSystems(Entity &entity) const {
    entitySystemRegistry.unregisterEntityFromSystems(entity);
}

void EntityContext::initializeEntities(EventManager &eventManager) {
    for (const auto &e: m_Entities) {
        if (e->m_Status == Entity::ACTIVE) {
            continue;
        }

        e->initializeComponents(*this);

        if (e->isReadyToRegister()) {
            registerEntityWithSystems(*e);
            e->setStatus(Entity::ACTIVE);
        }
    }
}

void EntityContext::registerEntityWithSystems(Entity &entity) const {
    entitySystemRegistry.registerEntityWithSystems(entity);
}

void EntityContext::initializeSystems(ResourceManager &resourceManager, EventManager &eventManager) {
    for (const auto &[system, processType]: m_systemInitializers) {
        system->m_EventManager = &eventManager; //Remove

        system->registerEventHandlers(eventManager);
        entitySystemRegistry.registerEntitySystem(*system, processType);
    }

    for (const auto &[system, processType]: m_systemInitializers) {
        system->initialize(resourceManager, eventManager);
    }

    postRegisterModules();

    m_systemInitializers.clear();
}

void EntityContext::processSystems(EventManager &eventManager) {
    initializeEntities(eventManager);

    entitySystemRegistry.processMain();
}

Entity *EntityContext::getEntity(const Identity::Type id) {
    for (const auto &e: m_Entities) {
        if (e->m_Id.id() == id) {
            return e.get();
        }
    }
    return nullptr;
}

Entity *EntityContext::findEntity(const std::string &name) {
    for (const auto &e: m_Entities) {
        if (e->m_Name == name) {
            return e.get();
        }
    }
    return nullptr;
}

void EntityContext::createComponentInplace(Identity::Type entityId, const std::string &componentName) {
    Entity *e = getEntity(entityId);
    if (e != nullptr) {
        Component *c = m_ComponentFactory.createInstance(componentName);
        if (c == nullptr) {
            Log::error("Component class not found " + componentName);

            return;
        }

        //TODO: Add is-unique check
        // if (e->getComponent(componentName) != nullptr) {
        //     delete c;
        //     Log::error("Entity already has component " + componentName);
        //
        //     return;
        // }

        c->m_Id = Identity::create(Identity::COMPONENT);
        c->m_EntityId = e->m_Id;

        unregisterEntityFromSystems(*e);
        e->addComponent(*c);
        e->setStatus(Entity::CREATED);
    }
}

void EntityContext::removeComponent(const Identity::Type entityId, const Identity::Type componentId) {
    Entity *e = getEntity(entityId);
    if (e != nullptr) {
        auto *c = e->getComponent(componentId);
        if (c != nullptr) {
            unregisterEntityFromSystems(*e);
            e->removeComponent(*c);
            e->setStatus(Entity::CREATED);
            //            delete c;
        }
    }
}

std::vector<std::string> EntityContext::getAllConfigurationNames() {
    return m_EntityConfiguration.getAllConfigurationNames();
}

std::vector<std::string> EntityContext::getAllComponentTypeNames() {
    return m_ComponentFactory.getNames();
}

Entity *EntityContext::findEntity(std::function<bool(Entity *)> functor) {
    auto it = std::find_if(
        m_Entities.begin(),
        m_Entities.end(),
        [&functor](const auto &v) {
            return functor(v.get());
        });

    if (it == m_Entities.end()) {
        return nullptr;
    }

    return it->get();
}
