#pragma once

#include "../Reflection/Identity.h"
#include "../System/EntitySystem.h"
#include "../Behaviour/EntityBehaviour.h"
#include <list>
#include <string>

class Component;

class EntityContext;

class Entity {
public:
    typedef std::shared_ptr<Entity> TEntityPtr;

    enum Status {
        CREATED,
        ACTIVE,
    };

    Entity();

    void addComponent(Component &);

    void removeComponent(Component &);

    void registerWithSystems(EntityContext &);

    void unregisterFromSystems(EntityContext &);

    void addBehaviour(EntityBehaviour &);

    void removeBehaviour(EntityBehaviour *);

    bool isReadyToRegister();

    void registerBehaviourEventHandlers(EventManager &eventManager);

    [[nodiscard]] std::string getListName() const {
        return m_Name;
    }

    template<class T>
    bool hasComponent() {
        for (const auto &c: m_Components) {
            if (dynamic_cast<T *>(c.get())) {
                return true;
            }
        }

        return false;
    }

    template<class T>
    T *getComponent() {
        for (const auto &c: m_Components) {
            if (dynamic_cast<T *>(c.get())) {
                return dynamic_cast<T *>(c.get());
            }
        }

        return nullptr;
    }

    Component *getComponent(const std::string &className) {
        for (const auto &c: m_Components) {
            if (c->m_Name == className || c->getTypeName() == className) {
                return c.get();
            }
        }

        return nullptr;
    }

    EntityBehaviour* findBehaviour(std::string typeName);

    Identity m_Id;
    std::string m_Name;
    Status m_Status;
    std::list<Component::TComponentPtr> m_Components;
    std::list<EntityBehaviour*> m_Behaviours;
    std::list<EntitySystem::TEntitySystemPtr> m_Systems;
};
