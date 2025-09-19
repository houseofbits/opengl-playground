#pragma once

#include <cassert>
#include "BaseSystemProcess.h"
#include <map>

class EntitySystem;
class BaseSystemProcess;
class ContinuousSystemProcess;

class EntitySystemRegistry {
public:
    typedef unsigned int ProcessType;

    static constexpr ProcessType MAIN_PROCESS = 0;

    EntitySystemRegistry() : m_eventManager(nullptr), m_systemProcesses() {
    }

    void setEventManager(EventManager &eventManager) {
        m_eventManager = &eventManager;
    }

    void registerEntityWithSystems(Entity &entity) const;

    void unregisterEntityFromSystems(Entity &entity) const;

    BaseSystemProcess &createMainProcess();

    ContinuousSystemProcess &createContinuousProcess(const ProcessType type, const long frequencyMs);

    template<class T>
    T &getProcess(const ProcessType type) {
        assert(m_systemProcesses.count(type) == 1);

        auto p = dynamic_cast<T *>(m_systemProcesses[type]);

        assert(p != nullptr);

        return *p;
    }

    void registerEntitySystem(EntitySystem &system, const ProcessType &type);

    void processMain();

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
