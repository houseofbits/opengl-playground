#pragma once
#include "EntitySystem.h"

class EntityContext;
class EventManager;
class Entity;

class BaseSystemProcess {
public:
    explicit BaseSystemProcess(EventManager &eventManager) : m_eventManager(&eventManager), m_Systems() {
    };

    virtual ~BaseSystemProcess() = default;

    void registerEntitySystem(EntitySystem &system) {
        m_Systems.push_back(&system);

        m_Systems.sort(
            [](const EntitySystem *a, const EntitySystem *b) {
                return a->m_processPriority < b->m_processPriority;
            });
    }

    void registerEntityWithSystems(Entity &entity) const {
        for (const auto &system: m_Systems) {
            system->registerEntityComponents(entity);
        }
    }

    virtual void process() {
        processAllSystems();
    }

protected:
    void processAllSystems() const {
        for (const auto &system: m_Systems) {
            system->process(*m_eventManager);
        }
    }

    EventManager *m_eventManager;
    std::list<EntitySystem *> m_Systems;
};
