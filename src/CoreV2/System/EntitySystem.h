#pragma once

#include "../Entity/Component.h"
#include "../Reflection/Type.h"
#include <memory>

class EventManager;
class EntityContext;
class ResourceManager;

class EntitySystem {
public:
    typedef std::shared_ptr<EntitySystem> TEntitySystemPtr;

    virtual void initialize(ResourceManager *) = 0;
    virtual void process() = 0;
    virtual void registerEventHandlers(EventManager *eventManager) {}

    class VComponentContainer {
    public:
        virtual void registerComponent(Component *) {};
        virtual void unregisterComponent(Component *) {};
    };

    template<class T>
    class RComponentContainer : public VComponentContainer {
    public:
        typedef std::unordered_map<Identity::Type, T *> ContainerType;

        void registerComponent(Component *comp) override {
            if (isOfType<T>(comp)) {
                m_components[comp->m_Id()] = dynamic_cast<T *>(comp);
            }
        }

        void unregisterComponent(Component *comp) override {
            m_components.erase(comp->m_EntityId.id());
        }

        T *get(Identity::Type entityId) {
            if (m_components.find(entityId) == m_components.end()) {
                return nullptr;
            }

            return m_components[entityId];
        }

        T *find(std::function<bool(T *)> functor) {
            auto it = std::find_if(
                    m_components.begin(),
                    m_components.end(),
                    [&functor](const auto &v) {
                        return functor(v.second);
                    });

            if (it == m_components.end()) {
                return nullptr;
            }

            return it->second;
        }

        ContainerType m_components;
    };

    std::unordered_map<unsigned int, VComponentContainer *> m_components;

    template<class T>
    void usesComponent() {
        m_components[T::TypeId()] = new RComponentContainer<T>();
    }

    virtual void registerComponent(Component *comp) {
        if (m_components.find(comp->getTypeId()) == m_components.end()) {
            return;
        }

        m_components[comp->getTypeId()]->registerComponent(comp);
    }

    virtual void unregisterComponent(Component *comp) {
        for (auto m: m_components) {
            m.second->unregisterComponent(comp);
        }
    }

    template<class T>
    T *getComponent(Identity::Type entityId) {
        return getContainer<T>()->get(entityId);
    }

    template<class T>
    T *findComponent(std::function<bool(T *)> functor) {
        return getContainer<T>()->find(functor);
    }

    template<class T>
    typename RComponentContainer<T>::ContainerType &getComponentContainer() {
        return getContainer<T>()->m_components;
    }


    EntityContext *m_EntityContext{nullptr};
    EventManager *m_EventManager{nullptr};

protected:
    template<class T>
    RComponentContainer<T> *getContainer() {
        assert(m_components.find(T::TypeId()) != m_components.end());
        assert(isOfType<RComponentContainer<T>>(m_components[T::TypeId()]));

        return ((RComponentContainer<T> *) m_components[T::TypeId()]);
    }
};