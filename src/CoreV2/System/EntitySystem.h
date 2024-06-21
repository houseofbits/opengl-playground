#pragma once

#include "../Entity/Component.h"
#include "../Events/EventManager.h"
#include <memory>

class EntitySystem {
public:
    typedef std::shared_ptr<EntitySystem> TEntitySystemPtr;

    virtual void registerComponent(Component*) = 0;
    virtual void unregisterComponent(Component*) = 0;
    virtual void initialize(ResourceManager*) = 0;
    virtual void process() = 0;
    virtual void registerEventHandlers(EventManager *eventManager) {}
};