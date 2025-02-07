#pragma once

#include "AbstractComponentRegistry.h"

template<class T>
class SameComponentRegistry final : public AbstractComponentRegistry {
public:
    typedef std::unordered_map<Identity::Type, T *> ContainerType;

    void registerComponents(Entity &entity) override {
        if (!entity.hasComponent<T>()) {
            return;
        }

        auto cv = entity.getAllComponents<T>();
        if (cv.size() > 0) {
            for (const auto c: cv) {
                m_container[c->getComponentId()] = c;
            }
        }
    }

    void registerComponent(Identity::Type id, T * &component) {
        m_container[id] = component;
    }

    //TODO: Unregister all entity comps
    void unregisterComponents(Identity::Type componentId) override {
        m_container.erase(componentId);
    }

    ContainerType &container() {
        return m_container;
    }

    T *get(Identity::Type componentId) {
        if (m_container.find(componentId) != m_container.end()) {
            return m_container[componentId];
        }

        return nullptr;
    }

    bool contains(Identity::Type componentId) override {
        return m_container.find(componentId) != m_container.end();
    }

    ContainerType m_container{};
};
