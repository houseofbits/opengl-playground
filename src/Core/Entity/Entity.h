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
    [[nodiscard]] bool hasComponent() const {
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

    template<class T>
    std::vector<T *> getAllComponents() {
        std::vector<T *> components;
        for (const auto &c: m_Components) {
            if (dynamic_cast<T *>(c.get())) {
                components.push_back(dynamic_cast<T *>(c.get()));
            }
        }

        return components;
    }

    [[nodiscard]] Component *getComponent(const std::string &className) const {
        for (const auto &c: m_Components) {
            if (c->getTypeName() == className) {
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
                    return dynamic_cast<T *>(c.get());
                }

                return nullptr;
            }
        }
        return nullptr;
    }

    template<class T>
    T *getComponent(const std::string &name) {
        for (const auto &c: m_Components) {
            if (c->m_Name == name) {
                if (dynamic_cast<T *>(c.get())) {
                    return dynamic_cast<T *>(c.get());
                }

                return nullptr;
            }
        }
        return nullptr;
    }

    [[nodiscard]] Component *findComponentByName(const std::string &name) const {
        for (const auto &c: m_Components) {
            if (c->m_Name == name) {
                return c.get();
            }
        }

        return nullptr;
    }

    [[nodiscard]] Component *findComponentByTypeName(const std::string &typeName) const {
        for (const auto &c: m_Components) {
            if (c->getTypeName() == typeName) {
                return c.get();
            }
        }

        return nullptr;
    }

    Identity m_Id;
    std::string m_Name;
    Status m_Status;
    std::list<Component::TComponentPtr> m_Components;
};
