#pragma once
#include "BaseSystemProcess.h"
#include "ContinuousSystemProcess.h"
#include "EntitySystem.h"

class EntitySystemRegistry {
public:
    typedef unsigned int ProcessType;

    static constexpr ProcessType MAIN_PROCESS = 0;

    EntitySystemRegistry() : m_eventManager(nullptr), m_systemProcesses() {
    }

    void setEventManager(EventManager &eventManager) {
        m_eventManager = &eventManager;
    }

    void registerEntityWithSystems(Entity &entity) const {
        for (const auto &[key, process]: m_systemProcesses) {
            process->registerEntityWithSystems(entity);
        }
    }

    BaseSystemProcess &createMainProcess() {
        assert(m_eventManager!= nullptr);

        auto p = new BaseSystemProcess(*m_eventManager);
        m_systemProcesses[MAIN_PROCESS] = p;

        return *p;
    }

    ContinuousSystemProcess &createContinuousProcess(const ProcessType type, const long frequencyMs) {
        assert(m_eventManager!= nullptr);
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

    void registerEntitySystem(EntitySystem &system, const ProcessType &type) {
        const auto process = m_systemProcesses[type];

        process->registerEntitySystem(system);
    }

    void processMain() {
        m_systemProcesses[MAIN_PROCESS]->process();
    }

    template<class T, typename = std::enable_if_t<std::is_base_of_v<EntitySystem, T> > >
    T *getSystem() {
        for (const auto &[type, process]: m_systemProcesses) {
            const auto system = process->getSystem<T>();
            if (system != nullptr) {
                return system;
            }
        }

        return nullptr;
    }

private:
    EventManager *m_eventManager;
    std::map<ProcessType, BaseSystemProcess *> m_systemProcesses;
};
