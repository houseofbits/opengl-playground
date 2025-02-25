#include "EntityContext.h"
#include "EntitySerializer.h"
#include <iostream>
#include <memory>

EntityContext::EntityContext() : m_ComponentFactory(),
                                 m_EntityConfiguration(m_ComponentFactory),
                                 m_Entities(),
                                 m_Systems(),
                                 m_entityBehavioursReadyToInitialize(),
                                 m_entityBehavioursForRemoval(),
                                 m_EntityBehaviourFactory() {
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

        Component *c = m_ComponentFactory.createInstance(key);
        if (c == nullptr) {
            Log::error("Component class not found " + key);
            continue;
        }

        c->m_Id = Identity::create(Identity::COMPONENT);
        c->m_Name = key;
        c->m_EntityId = e->m_Id;

        e->addComponent(*c);
    }

    if (entityJson.contains("behaviours") && entityJson["behaviours"].is_object()) {
        nlohmann::json behavioursJson = entityJson["behaviours"];
        for (auto &[key, value]: behavioursJson.items()) {
            EntityBehaviour *b = m_EntityBehaviourFactory.createInstance(key);
            if (b == nullptr) {
                Log::error("Behaviour class not found ", key);
                continue;
            }

            b->setEntity(e.get());
            e->addBehaviour(*b);
        }
    }

    return e;
}

void EntityContext::removeEntity(int entityId) {
    Entity *e = getEntity(entityId);
    if (e != nullptr) {
        e->unregisterFromSystems(*this);

        for (const auto &entity: m_Entities) {
            if (entity->m_Id.id() == entityId) {
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

void EntityContext::deserializeEntities(nlohmann::json &j, ResourceManager &resourceManager) {
    for (const auto &entityJson: j.items()) {
        Entity::TEntityPtr entity = createEntityFromJson(entityJson.value());
        EntitySerializer::deserialize(*entity, entityJson.value(), resourceManager);
    }
}

void EntityContext::unregisterComponentFromSystems(Component *component) {
    for (const auto &system: m_Systems) {
        system->unregisterComponent(component);
    }
}

void EntityContext::registerEntitiesWithSystems(EventManager &eventManager) {
    for (const auto &e: m_Entities) {
        if (e->m_Status == Entity::CREATED && e->isReadyToRegister()) {
            if (!e->m_Behaviours.empty()) {
                for (const auto &behaviour: e->m_Behaviours) {
                    behaviour->setEventManager(&eventManager);
                    behaviour->registerEventHandlers(eventManager);
                }
            }

            e->registerWithSystems(*this);
        }
    }
}

void EntityContext::initializeSystems(ResourceManager &resourceManager, EventManager &eventManager) {
    m_Systems.sort(
            [](const EntitySystem *a, const EntitySystem *b) { return a->m_processPriority < b->m_processPriority; });

    for (const auto &system: m_Systems) {
        system->m_EventManager = &eventManager;
        system->initialize(resourceManager);
        system->registerEventHandlers(eventManager);
    }
}

void EntityContext::processSystems(EventManager &eventManager) {
    registerEntitiesWithSystems(eventManager);

    for (const auto &system: m_Systems) {
        system->process(eventManager);
    }
}

Entity *EntityContext::getEntity(Identity::Type id) {
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

void EntityContext::createComponentInplace(Identity::Type entityId, const std::string& componentName) {
    Entity *e = getEntity(entityId);
    if (e != nullptr) {
        Component *c = m_ComponentFactory.createInstance(componentName);
        if (c == nullptr) {
            Log::error("Component class not found " + componentName);

            return;
        }

        if (e->getComponent(componentName) != nullptr) {
            delete c;
            Log::error("Entity already has component " + componentName);

            return;
        }

        c->m_Id = Identity::create(Identity::COMPONENT);
        c->m_Name = componentName;
        c->m_EntityId = e->m_Id;

        e->addComponent(*c);
        c->registerWithSystems(*this);
    }
}

void EntityContext::removeComponent(Identity::Type entityId, const std::string& componentName) {
    Entity *e = getEntity(entityId);
    if (e != nullptr) {
        auto *c = e->getComponent(componentName);
        if (c != nullptr) {
            unregisterComponentFromSystems(c);
            e->removeComponent(*c);
            delete c;
        }
    }
}

std::vector<std::string> EntityContext::getBehaviourTypes() {
    return m_EntityBehaviourFactory.getNames();
}

void EntityContext::addBehaviour(Identity::Type entityId, const std::string& type) {
    auto *e = getEntity(entityId);
    if (e == nullptr) {
        return;
    }

    if (e->findBehaviour(type) != nullptr) {
        Log::warn("Cannot add behaviour. Behaviour already exists on entity");

        return;
    }

    auto *b = m_EntityBehaviourFactory.createInstance(type);
    if (b == nullptr) {
        return;
    }

    b->setEntity(e);
    e->addBehaviour(*b);
    m_entityBehavioursReadyToInitialize.push_back(b);
}

void EntityContext::removeBehaviour(Identity::Type entityId, const std::string& type) {
    auto *e = getEntity(entityId);
    if (e == nullptr) {
        return;
    }

    auto b = e->findBehaviour(type);
    if (b == nullptr) {
        Log::warn("Cannot remove behaviour. Behaviour not found on entity");

        return;
    }

    m_entityBehavioursForRemoval.push_back(b);
}

void EntityContext::processBehaviours(ResourceManager &resourceManager, EventManager &eventManager) {
    for (const auto &behaviour: m_entityBehavioursReadyToInitialize) {
        behaviour->initialize(resourceManager);
        behaviour->registerEventHandlers(eventManager);
    }
    m_entityBehavioursReadyToInitialize.clear();

    for (const auto &behaviour: m_entityBehavioursForRemoval) {
        eventManager.removeEventHandler((EventHandler *) behaviour);
        behaviour->detachFromEntity();
        delete behaviour;
    }
    m_entityBehavioursForRemoval.clear();
}
