#include "ApplicationSystem.h"
#include "../../../Core/Entity/EntitySerializer.h"
#include "../../../Core/Helper/Time.h"
#include <fstream>

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
    eventManager.registerEventReceiver(this, &ApplicationSystem::handleEditorUIEvent);
    eventManager.registerEventReceiver(this, &ApplicationSystem::handleEntityPersistenceEvent);
}

void ApplicationSystem::handleEntityCreationEvent(const EntityCreationEvent &event) {
    if (event.m_Type == EntityCreationEvent::CREATE) {
        auto e = m_EntityContext->createEntityFromTemplate(event.m_ConfigurationName, *m_ResourceManager);
        e->m_Name = event.m_name;
        nlohmann::basic_json json;
        // EntitySerializer::deserialize(*e, json, *m_ResourceManager);
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
        m_EntityContext->removeComponent(event.m_entityId, event.m_componentId);
    }
}

void ApplicationSystem::handleEditorUIEvent(const EditorUIEvent &event) {
    if (event.m_Type == EditorUIEvent::SAVE) {
        saveEntitiesToFile();
    }

    if (event.m_Type == EditorUIEvent::LOAD) {
        loadEntitiesFromFile("data/scenes/physics.json");
    }
}

void ApplicationSystem::handleEntityPersistenceEvent(const EntityPersistenceEvent &event) {
    if (event.m_Type == EntityPersistenceEvent::TYPE_SAVE) {
        saveEntitiesToFile();
    }

    if (event.m_Type == EntityPersistenceEvent::TYPE_LOAD) {
        loadEntitiesFromFile(event.m_path);
    }
}

void ApplicationSystem::setTime() {
    float currentTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;

    Time::frameTime = currentTime - Time::timestamp;
    Time::timestamp = currentTime;
}

void ApplicationSystem::loadEntitiesFromFile(const std::string &fileName) {
    m_EntitySourceFileName = fileName;
    std::ifstream file(fileName);
    if (file.fail()) {
        Log::error("Application::loadEntitiesFromFile: Failed to read " + fileName);
        return;
    }
    auto json = nlohmann::json::parse(file);
    m_EntityContext->deserializeEntities(json, *m_ResourceManager);
}

void ApplicationSystem::saveEntitiesToFile() const {
    nlohmann::json j;
    m_EntityContext->serializeEntities(j);

    std::ofstream file;
    file.open(m_EntitySourceFileName);

    std::string data(j.dump(4));
    file << data;
}
