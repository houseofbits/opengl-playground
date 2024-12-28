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

    template<class T, typename = std::enable_if_t<std::is_base_of_v<EntitySystem, T>> >
    T *getSystem() {
        for (const auto &elem: m_Systems) {
            if (dynamic_cast<T *>(elem)) {
                return dynamic_cast<T *>(elem);
            }
        }

        return nullptr;
    }

protected:
    void processAllSystems() const {
        for (const auto &system: m_Systems) {
            system->processEvents();
            system->process(*m_eventManager);
        }
    }

    EventManager *m_eventManager;
    std::list<EntitySystem *> m_Systems;
};
