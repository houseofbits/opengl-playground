#pragma once

class EntityContext;

class EntityModule {
public:
    EntityModule();

    virtual void registerComponents(EntityContext&) {};
    virtual void registerSystems(EntityContext&) {};
};
