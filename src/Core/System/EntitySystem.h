#pragma once

#include "../Entity/Component.h"
#include "../Reflection/Type.h"
#include "../Events/EventManager.h"
#include "AbstractComponentRegistry.h"
#include "RelatedComponentRegistry.h"
#include "SingleComponentRegistry.h"
#include <memory>

class EventManager;
class EntityContext;
class ResourceManager;
class Entity;

class EntitySystem : public EventHandler {
public:
    EntitySystem() : EventHandler() {}

    typedef std::shared_ptr<EntitySystem> TEntitySystemPtr;

    /**
     * Initialize is run for all systems once on startup. After the render context
     * is created and before entities are deserialized
     */
    virtual void initialize(ResourceManager &) {};
    /**
     * Process runs each frame for each system
     */
    virtual void process(EventManager &) {};
    virtual void registerEventHandlers(EventManager &) {}

    //Used for event manager to identify target handlers
    Identity::Type getId() override {
        return 0;
    }

    EntityContext *m_EntityContext{nullptr};
    EventManager *m_EventManager{nullptr};
    unsigned int m_processPriority{};

    template<class... Ts>
    RelatedComponentRegistry<Ts...> *useComponentsRegistry() {
        auto reg = new RelatedComponentRegistry<Ts...>();
        m_registries.push_back(reg);

        return reg;
    }

    template<class T>
    SingleComponentRegistry<T> *useComponentRegistry() {
        auto reg = new SingleComponentRegistry<T>();
        m_registries.push_back(reg);

        return reg;
    }

    void registerEntityComponents(Entity &entity) {
        for (const auto &reg: m_registries) {
            reg->registerComponents(entity);
        }
    }

    std::list<AbstractComponentRegistry *> m_registries;
};