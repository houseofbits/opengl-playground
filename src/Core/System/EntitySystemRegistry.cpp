#include "./EntitySystemRegistry.h"
#include "./BaseSystemProcess.h"
#include "./ContinuousSystemProcess.h"

void EntitySystemRegistry::processMain() {
    m_systemProcesses[MAIN_PROCESS]->process();
}

void EntitySystemRegistry::registerEntitySystem(EntitySystem &system, const ProcessType &type) {
    const auto process = m_systemProcesses[type];

    process->registerEntitySystem(system);
}

void EntitySystemRegistry::registerEntityWithSystems(Entity &entity) const {
    for (const auto &[key, process]: m_systemProcesses) {
        process->registerEntityWithSystems(entity);
    }
}

BaseSystemProcess &EntitySystemRegistry::createMainProcess() {
    assert(m_eventManager!= nullptr);

    auto p = new BaseSystemProcess(*m_eventManager);
    m_systemProcesses[MAIN_PROCESS] = p;

    return *p;
}

ContinuousSystemProcess &EntitySystemRegistry::createContinuousProcess(const ProcessType type, const long frequencyMs) {
    assert(m_eventManager!= nullptr);
    assert(type != MAIN_PROCESS);
    assert(m_systemProcesses.count(type) == 0);

    auto p = new ContinuousSystemProcess(*m_eventManager, frequencyMs);
    m_systemProcesses[type] = p;

    return *p;
}