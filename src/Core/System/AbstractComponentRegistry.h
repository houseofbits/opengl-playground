#pragma once

class Entity;

class AbstractComponentRegistry {
public:
    virtual ~AbstractComponentRegistry() = default;

    virtual void registerComponents(Entity &entity) = 0;
    // virtual void unregisterComponents(Identity::Type entityId) = 0;
    virtual void unregisterComponents(Entity &entity) = 0;
    virtual bool contains(Identity::Type id) = 0;
};
