#include "EditorApplication.h"
#include "../../Core/Entity/EntitySerializer.h"
#include "../../Modules/Common/CommonModule.h"
#include "../../Modules/Editor/EditorUIModule.h"
#include "../../Modules/Physics/PhysicsModule.h"
#include "../../Modules/Renderer/RendererModule.h"
#include "../../Modules/Behaviours/BehavioursModule.h"
#include "../../Modules/Application/ApplicationModule.h"
#include "../../Modules/Application/Events/SystemEvent.h"
#include <fstream>

EditorApplication::EditorApplication() : Application(), EventListener() {
    m_EventManager.registerEventReceiver(this, &EditorApplication::handleEditorUIEvent);
}

void EditorApplication::initialize(const std::string &entityDefinitionFileName) {
    std::ifstream file(entityDefinitionFileName);
    if (file.fail()) {
        Log::error("Application::Application: Failed to read " + entityDefinitionFileName);
        return;
    }
    auto json = nlohmann::json::parse(file);
    m_EntityContext.deserializeEntityMap(json);

    m_EntityContext.registerModule<ApplicationModule>();
    m_EntityContext.registerModule<CommonModule>();
    m_EntityContext.registerModule<RendererModule>();
    m_EntityContext.registerModule<EditorUIModule>();
    m_EntityContext.registerModule<PhysicsModule>();
    m_EntityContext.registerModule<BehavioursModule>();

    m_EntityContext.initializeSystems(m_ResourceManager, m_EventManager);

    m_EventManager.queueEvent<SystemEvent>(SystemEvent::ENTITY_SYSTEMS_READY);
}

void EditorApplication::run() {
    auto applicationEventsSystem = m_EntityContext.getSystem<ApplicationEventsSystem>();

    while (applicationEventsSystem->isRunning) {
        m_ResourceManager.buildFetchedResources();
        m_EntityContext.processSystems(m_EventManager);
    }
}

void EditorApplication::handleEditorUIEvent(const EditorUIEvent &event) {
    if (event.m_Type == EditorUIEvent::SAVE) {
        saveEntitiesToFile();
    }
}
