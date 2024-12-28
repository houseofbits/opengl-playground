#include "MainApplication.h"
#include "../../Core/Entity/EntitySerializer.h"
#include "../../Modules/Common/CommonModule.h"
#include "../../Modules/Physics/PhysicsModule.h"
#include "../../Modules/Renderer/RendererModule.h"
#include "../../Modules/Behaviours/BehavioursModule.h"
#include "../../Modules/Application/ApplicationModule.h"
#include "../../Modules/Application/Events/SystemEvent.h"

MainApplication::MainApplication() : Application() {
}

void MainApplication::initialize(const std::string &entityDefinitionFileName) {
    m_EntityContext.registerModule<ApplicationModule>();
    m_EntityContext.registerModule<CommonModule>();
    m_EntityContext.registerModule<RendererModule>();
    m_EntityContext.registerModule<PhysicsModule>();
    m_EntityContext.registerModule<BehavioursModule>();
    m_EntityContext.initializeSystems(m_ResourceManager, m_EventManager);
    m_EventManager.queueEvent<SystemEvent>(SystemEvent::ENTITY_SYSTEMS_READY);
}

void MainApplication::run() {
    auto applicationEventsSystem = m_EntityContext.getSystem<ApplicationEventsSystem>();
    while (applicationEventsSystem->isRunning) {
        m_ResourceManager.buildFetchedResources();
        m_EntityContext.processSystems(m_EventManager);
    }
}
