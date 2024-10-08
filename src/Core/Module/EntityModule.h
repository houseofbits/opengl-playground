#pragma once

class EntityContext;

class EntityModule {
public:
    EntityModule();

    virtual void registerComponents(EntityContext &) {};

    virtual void registerBehaviours(EntityContext &) {};

    virtual void registerSystems(EntityContext &) {};
};
