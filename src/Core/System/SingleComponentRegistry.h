#pragma once

#include "AbstractComponentRegistry.h"

template<class T>
class SingleComponentRegistry : public AbstractComponentRegistry {
public:
    typedef std::unordered_map<Identity::Type, T *> ContainerType;

    void registerComponents(Entity &entity) override {
        if (!entity.hasComponent<T>()) {
            return;
        }

        m_container[entity.m_Id.id()] = entity.getComponent<T>();
    }

    ContainerType &container() {
        return m_container;
    }

    T *get(Identity::Type entityId) {
        if (m_container.find(entityId) != m_container.end()) {
            return m_container[entityId];
        }

        return nullptr;
    }

    ContainerType m_container{};
};