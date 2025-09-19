#include "EditorApplication.h"
// #include "../../Core/Entity/EntitySerializer.h"
#include "../../Modules/Common/CommonModule.h"
#include "../../Modules/Editor/EditorUIModule.h"
#include "../../Modules/Physics/PhysicsModule.h"
#include "../../Modules/Renderer/RendererModule.h"
#include "../../Modules/Behaviours/BehavioursModule.h"
#include "../../Modules/Application/ApplicationModule.h"
#include "../../Modules/Application/Events/SystemEvent.h"
#include <fstream>

EditorApplication::EditorApplication() : Application() {
}

EditorApplication::~EditorApplication() = default;

void EditorApplication::initialize(const std::string &fileName, const std::string &entityDefinitionFileName) {
    auto json = Json::readFile(entityDefinitionFileName);
    if (!json) {
        return;
    }
    m_EntityContext.deserializeEntityMap(json.value());

    m_EntityContext.scriptingManager.init();

    m_EntityContext.registerModule<ApplicationModule>();
    m_EntityContext.registerModule<CommonModule>();
    m_EntityContext.registerModule<RendererModule>();
    m_EntityContext.registerModule<EditorUIModule>();
    m_EntityContext.registerModule<PhysicsModule>();
    m_EntityContext.registerModule<BehavioursModule>();
    m_EntityContext.initializeSystems(m_ResourceManager, m_EventManager);
    m_EventManager.queueEvent<SystemEvent>(SystemEvent::ENTITY_SYSTEMS_READY);
    m_EventManager.queueEvent<EntityPersistenceEvent>(EntityPersistenceEvent::TYPE_LOAD, fileName);
    m_EventManager.queueEvent<SystemEvent>(SystemEvent::REQUEST_EDITOR_MODE);
}

void EditorApplication::run() {
    while (m_EntityContext.getSystem<ApplicationEventsSystem>()->isRunning) {
        m_ResourceManager.buildFetchedResources();
        m_EntityContext.processSystems(m_EventManager);
    }
}
