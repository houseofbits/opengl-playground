#include "BaseSystemProcess.h"
#include "EntitySystem.h"

void BaseSystemProcess::registerEntitySystem(EntitySystem &system) {
    m_Systems.push_back(&system);

    m_Systems.sort(
        [](const EntitySystem *a, const EntitySystem *b) {
            return a->m_processPriority < b->m_processPriority;
        });
}

void BaseSystemProcess::registerEntityWithSystems(Entity &entity) const {
    for (const auto &system: m_Systems) {
        system->registerEntityComponents(entity);
    }
}

void BaseSystemProcess::unregisterEntityFromSystems(Entity &entity) const {
    for (const auto &system: m_Systems) {
        system->unregisterEntityComponents(entity);
    }
}

void BaseSystemProcess::process() {
    processAllSystems();
}

void BaseSystemProcess::processAllSystems() const {
    for (const auto &system: m_Systems) {
        system->processEvents();
        system->process(*m_eventManager);
    }
}