#include "MainApplication.h"
#include "../../Modules/Common/CommonModule.h"
#include "../../Modules/EditorUI/EditorUIModule.h"
#include "../../Modules/Physics/PhysicsModule.h"
#include "../../Modules/Renderer/RendererModule.h"
#include <fstream>

MainApplication::MainApplication() : Application(), m_Window(&m_EventManager) {
    m_EventManager.registerEventReceiver(this, &MainApplication::handleInputEvent);
    m_EventManager.registerEventReceiver(this, &MainApplication::handleEditorUIEvent);
    m_EventManager.registerEventReceiver(this, &MainApplication::handleEntityCreationEvent);
}

void MainApplication::initialize(const std::string &entityDefinitionFileName) {
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

    m_Window.create();

    m_EntityContext.initializeSystems(&m_ResourceManager, &m_EventManager);
}

void MainApplication::run() {
    while (true) {
        m_ResourceManager.buildFetchedResources();
        if (!m_Window.pollEvents() || !m_EventManager.processEvents()) {
            break;
        }

        m_EntityContext.processSystems();

        m_Window.doubleBuffer();
    }

    m_Window.destroy();
}

bool MainApplication::handleInputEvent(InputEvent *const event) {
    return true;
}

bool MainApplication::handleEditorUIEvent(EditorUIEvent *event) {
    if (event->m_Type == EditorUIEvent::SAVE) {
        saveEntitiesToFile();
    }

    return true;
}

bool MainApplication::handleEntityCreationEvent(EntityCreationEvent *event) {
    if (event->m_Type == EntityCreationEvent::CREATE) {
        auto e = m_EntityContext.createEntity(event->m_ConfigurationName, m_ResourceManager);
        e->m_Name = event->m_name;
    }
    if (event->m_Type == EntityCreationEvent::REMOVE) {
        m_EntityContext.removeEntity(event->m_entityId);
    }
    return true;
}
