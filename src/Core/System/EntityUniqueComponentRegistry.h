#pragma once

#include "AbstractComponentRegistry.h"

template<class T>
class EntityUniqueComponentRegistry final : public AbstractComponentRegistry {
public:
    typedef std::unordered_map<Identity::Type, T *> ContainerType;

    void registerComponents(Entity &entity) override {
        if (!entity.hasComponent<T>()) {
            return;
        }

        m_container[entity.m_Id.id()] = entity.getComponent<T>();
    }

    void registerComponent(Identity::Type id, T * &component) {
        m_container[id] = component;
    }

    void unregisterComponents(Entity &entity) override {
        m_container.erase(entity.m_Id.id());
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

    bool contains(Identity::Type id) override {
        return m_container.find(id) != m_container.end();
    }

    ContainerType m_container{};
};
