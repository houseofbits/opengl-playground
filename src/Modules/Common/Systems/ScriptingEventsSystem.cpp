#include "ScriptingEventsSystem.h"

ScriptingEventsSystem::ScriptingEventsSystem() : EntitySystem() {
}

void ScriptingEventsSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &ScriptingEventsSystem::handleCharacterPickingEvent);
    eventManager.registerEventReceiver(this, &ScriptingEventsSystem::handlePhysicsTriggerShapeEvent);
}

void ScriptingEventsSystem::handleCharacterPickingEvent(const PhysicsPickingEvent &event) {
    m_EntityContext->scriptingManager.handleEvent("PhysicsPickingEvent", event);
}

void ScriptingEventsSystem::handlePhysicsTriggerShapeEvent(const PhysicsTriggerShapeEvent &event) {
    m_EntityContext->scriptingManager.handleEvent("PhysicsTriggerShapeEvent", event);
}
