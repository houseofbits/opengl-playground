#pragma once

#include "../Reflection/Identity.h"
#include "../System/EntitySystem.h"
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

    Identity m_Id;
    std::string m_Name;
    Status m_Status = CREATED;
    std::list<Component::TComponentPtr> m_Components;
    std::list<EntitySystem::TEntitySystemPtr> m_Systems;

    void addComponent(Component &);
    void removeComponent(Component &);
    void registerWithSystems(EntityContext &);
    void unregisterFromSystems(EntityContext &);
    bool isReadyToRegister();
    [[nodiscard]] std::string getListName() const {
        return m_Name;
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

    Component *getComponent(std::string className) {
        for (const auto &c: m_Components) {
            if (c->m_Name == className || c->getTypeName() == className) {
                return c.get();
            }
        }

        return nullptr;
    }
};
