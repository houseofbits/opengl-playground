#pragma once
#include "BaseSystemProcess.h"
#include "ContinuousSystemProcess.h"
#include "EntitySystem.h"

class EntitySystemRegistry {
public:
    typedef unsigned int ProcessType;

    static const ProcessType MAIN_PROCESS = 0;

    EntitySystemRegistry();

    void initialize(EventManager &eventManager) {
        m_eventManager = &eventManager;

        createMainProcess();
    }

    void registerEntityWithSystems(Entity &entity) const {
        for (const auto &[key, process]: m_systemProcesses) {
            process->registerEntityWithSystems(entity);
        }
    }

    BaseSystemProcess &createMainProcess() {
        auto p = new BaseSystemProcess(*m_eventManager);
        m_systemProcesses[MAIN_PROCESS] = p;

        return *p;
    }

    ContinuousSystemProcess &createContinuousProcess(const ProcessType type, const long frequencyMs) {

        assert(type != MAIN_PROCESS);
        assert(m_systemProcesses.count(type) == 0);

        auto p = new ContinuousSystemProcess(*m_eventManager, frequencyMs);
        m_systemProcesses[type] = p;

        return *p;
    }

    template<class T>
    T &getProcess(const ProcessType type) {
        assert(m_systemProcesses.count(type) == 1);

        auto p = dynamic_cast<T *>(m_systemProcesses[type]);

        assert(p != nullptr);

        return *p;
    }

    void registerEntitySystem(EntitySystem& system, const ProcessType& type) {
        const auto process = m_systemProcesses[type];

        process->registerEntitySystem(system);
    }

    void processMain() {
        m_systemProcesses[MAIN_PROCESS]->process();
    }

private:
    EventManager *m_eventManager;
    std::map<ProcessType, BaseSystemProcess *> m_systemProcesses;
};
