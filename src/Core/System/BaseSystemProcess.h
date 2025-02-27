#pragma once
// #include "EntitySystem.h"
#include <list>

class EntitySystem;
class EntityContext;
class EventManager;
class Entity;

class BaseSystemProcess {
public:
    explicit BaseSystemProcess(EventManager &eventManager) : m_eventManager(&eventManager), m_Systems() {
    };

    virtual ~BaseSystemProcess() = default;

    void registerEntitySystem(EntitySystem &system);

    void registerEntityWithSystems(Entity &entity) const;

    void unregisterEntityFromSystems(Entity &entity) const;

    virtual void process();

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
    void processAllSystems() const;

    EventManager *m_eventManager;
    std::list<EntitySystem *> m_Systems;
};
