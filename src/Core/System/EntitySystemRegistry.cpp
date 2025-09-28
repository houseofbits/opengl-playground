#include "./EntitySystemRegistry.h"
#include "./BaseSystemProcess.h"
#include "./ContinuousSystemProcess.h"
#include "EntitySystem.h"
#include "../Helper/Log.h"

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

void EntitySystemRegistry::unregisterEntityFromSystems(Entity &entity) const {
    for (const auto &[key, process]: m_systemProcesses) {
        process->unregisterEntityFromSystems(entity);
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

void EntitySystemRegistry::printProfilingData() {

    for (const auto &[key, process]: m_systemProcesses) {
        auto processInfo = getProcessProfilingInfo(key);

        Log::write("Process ID ", processInfo.id, " ", processInfo.timeMs, "ms ", processInfo.perSecond, "fps");

        auto systemsInfo = getProcessSystemsProfilingInfo(processInfo);
        for (const auto &system: systemsInfo) {
            Log::write(system.timeMs, "ms ", system.percent, "% - ", system.systemName);
        }
    }
}

EntitySystemRegistry::ProcessProfilingInfo EntitySystemRegistry::getProcessProfilingInfo(ProcessType processId) {
    double systemTotal = 0;
    float perSecond = 0;
    for (const auto &[key, process]: m_systemProcesses) {
        if (processId != key) {
            continue;
        }

        for (const auto &system: process->m_Systems) {
            systemTotal += system->getProfilingTime();
        }
    }

    if (systemTotal > 0) {
        perSecond = 1000.0 / systemTotal;
    }

    return ProcessProfilingInfo(processId, systemTotal, perSecond);
}

std::vector<EntitySystemRegistry::EntitySystemProfilingInfo> EntitySystemRegistry::getProcessSystemsProfilingInfo(
    const ProcessProfilingInfo &systemInfo) {
    std::vector<EntitySystemRegistry::EntitySystemProfilingInfo> result;

    for (const auto &[key, process]: m_systemProcesses) {
        if (systemInfo.id != key) {
            continue;
        }

        for (const auto &system: process->m_Systems) {
            auto time = system->getProfilingTime();
            float percent = (time / systemInfo.timeMs) * 100.0;

            result.push_back(EntitySystemProfilingInfo(system->m_name, time, percent));
        }
    }

    return result;
}
