#pragma once

class EntityContext;

class EntityModule {
public:
    virtual ~EntityModule() = default;

    EntityModule();

    virtual void registerComponents(EntityContext &) {};

    virtual void registerSystems(EntityContext &) {};

    virtual void postRegister(EntityContext &) {};
};
