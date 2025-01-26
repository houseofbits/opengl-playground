#pragma once

#include "../Reflection/Identity.h"
#include "./Component.h"
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

    void removeComponent(const Component &);

    void setStatus(Status);

    bool isReadyToRegister();

    void initializeComponents(EntityContext &) const;

    [[nodiscard]] std::string getListName() const {
        return m_Name; // + " (" + std::to_string(m_Id.id()) + ")";
    }

    template<class T>
    bool hasComponent() const {
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

    [[nodiscard]] Component *getComponent(const std::string &className) const {
        for (const auto &c: m_Components) {
            if (c->m_Name == className || c->getTypeName() == className) {
                return c.get();
            }
        }

        return nullptr;
    }

    [[nodiscard]] Component *getComponent(const Identity::Type componentId) const {
        for (const auto &c: m_Components) {
            if (c->m_Id.id() == componentId) {
                return c.get();
            }
        }

        return nullptr;
    }

    template<class T>
    T *getComponent(const Identity::Type componentId) {
        for (const auto &c: m_Components) {
            if (c->m_Id.id() == componentId) {
                if (dynamic_cast<T *>(c.get())) {
                    return c;
                }

                return nullptr;
            }
        }
        return nullptr;
    }

    Identity m_Id;
    std::string m_Name;
    Status m_Status;
    std::list<Component::TComponentPtr> m_Components;
};
