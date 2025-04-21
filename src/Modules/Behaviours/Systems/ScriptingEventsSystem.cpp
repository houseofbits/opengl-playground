#include "ScriptingEventsSystem.h"

ScriptingEventsSystem::ScriptingEventsSystem() : EntitySystem() {
    m_registry = useSameComponentRegistry<ScriptingComponent>();
}

void ScriptingEventsSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &ScriptingEventsSystem::handlePhysicsPickingEvent);
    eventManager.registerEventReceiver(this, &ScriptingEventsSystem::handlePhysicsSensorEvent);
    eventManager.registerEventReceiver(this, &ScriptingEventsSystem::handleInputEvent);
    eventManager.registerEventReceiver(this, &ScriptingEventsSystem::handleSystemEvent);
}

void ScriptingEventsSystem::handlePhysicsPickingEvent(const PhysicsPickingEvent &event) {
    if (m_isSimulationDisabled) {
        return;
    }
    if (event.m_doActivate) {
        m_EntityContext->scriptingManager.handleEvent("PhysicsPickingEvent", event);
    }
}

void ScriptingEventsSystem::handlePhysicsSensorEvent(const PhysicsSensorEvent &event) {
    if (m_isSimulationDisabled) {
        return;
    }
    m_EntityContext->scriptingManager.handleEvent("PhysicsSensorEvent", event);
}

void ScriptingEventsSystem::handleInputEvent(const InputEvent &event) {
    if (m_isSimulationDisabled) {
        return;
    }
    if (event.type == InputEvent::KEYUP) {
        m_EntityContext->scriptingManager.handleEvent("InputEvent", event);
    }
}

void ScriptingEventsSystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        m_isSimulationDisabled = false;
        runAllScripts();
    } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
        m_isSimulationDisabled = true;
    }
}

void ScriptingEventsSystem::runAllScripts() const {
    for (const auto &[componentId, component]: m_registry->container()) {
        if (!component->m_scriptFile.empty()) {
            m_EntityContext->scriptingManager.runScriptFromFile(component->m_scriptFile);
        }
        if (!component->m_scriptSource.empty()) {
            m_EntityContext->scriptingManager.runScript(component->m_scriptSource);
        }
    }
}
