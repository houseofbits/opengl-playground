#include "EditorApplication.h"
#include "../../Core/Entity/EntitySerializer.h"
#include "../../Modules/Common/CommonModule.h"
#include "../../Modules/Editor/EditorUIModule.h"
#include "../../Modules/Physics/PhysicsModule.h"
#include "../../Modules/Renderer/RendererModule.h"
#include "../../Modules/Behaviours/BehavioursModule.h"
#include <fstream>

EditorApplication::EditorApplication() : Application(), EventHandler(), m_Window(&m_EventManager) {
    m_EventManager.registerEventReceiver(this, &EditorApplication::handleInputEvent);
    m_EventManager.registerEventReceiver(this, &EditorApplication::handleEditorUIEvent);
    m_EventManager.registerEventReceiver(this, &EditorApplication::handleEntityCreationEvent);
}

void EditorApplication::initialize(const std::string &entityDefinitionFileName) {
    std::ifstream file(entityDefinitionFileName);
    if (file.fail()) {
        Log::error("Application::Application: Failed to read " + entityDefinitionFileName);
        return;
    }
    auto json = nlohmann::json::parse(file);
    m_EntityContext.deserializeEntityMap(json);

    m_EntityContext.registerModule<CommonModule>();
    m_EntityContext.registerModule<RendererModule>();
    m_EntityContext.registerModule<EditorUIModule>();
    m_EntityContext.registerModule<PhysicsModule>();
    m_EntityContext.registerModule<BehavioursModule>();

    m_Window.create();

    m_EntityContext.initializeSystems(m_ResourceManager, m_EventManager);
}

void EditorApplication::run() {
    while (true) {
        m_ResourceManager.buildFetchedResources();
        if (!m_Window.pollEvents()) {
            break;
        }

        m_EventManager.processEvents();
        m_EntityContext.processSystems(m_EventManager);

        m_Window.doubleBuffer();
    }

    m_Window.destroy();
}

void EditorApplication::handleInputEvent(const InputEvent &) {

}

void EditorApplication::handleEditorUIEvent(const EditorUIEvent &event) {
    if (event.m_Type == EditorUIEvent::SAVE) {
        saveEntitiesToFile();
    }
}

void EditorApplication::handleEntityCreationEvent(const EntityCreationEvent &event) {
    if (event.m_Type == EntityCreationEvent::CREATE) {
        auto e = m_EntityContext.createEntityFromTemplate(event.m_ConfigurationName, m_ResourceManager);
        e->m_Name = event.m_name;
        nlohmann::basic_json json;
        EntitySerializer::deserialize(*e, json, m_ResourceManager);
    }
    if (event.m_Type == EntityCreationEvent::CLONE) {
        auto existingEntity = m_EntityContext.getEntity(event.m_entityId);
        if (existingEntity != nullptr) {
            nlohmann::basic_json json;
            EntitySerializer::serialize(*existingEntity, json);
            auto e = m_EntityContext.createEntityFromJson(json);
            EntitySerializer::deserialize(*e, json, m_ResourceManager);
            e->m_Name = existingEntity->m_Name + "-COPY";
        }
    }
    if (event.m_Type == EntityCreationEvent::REMOVE) {
        m_EntityContext.removeEntity(event.m_entityId);
    }
    if (event.m_Type == EntityCreationEvent::CREATE_COMPONENT) {
        m_EntityContext.createComponentInplace(event.m_entityId, event.m_name);
    }
    if (event.m_Type == EntityCreationEvent::REMOVE_COMPONENT) {
        m_EntityContext.removeComponent(event.m_entityId, event.m_name);
    }
}
