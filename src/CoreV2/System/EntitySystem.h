#pragma once

#include <memory>
#include "../Entity/Component.h"

class EntitySystem {
public:
    typedef std::shared_ptr<EntitySystem> TEntitySystemPtr;

    virtual void registerComponent(Component*) = 0;
    virtual void unregisterComponent(Component*) = 0;
    virtual void initialize() = 0;
    virtual void process() = 0;
};