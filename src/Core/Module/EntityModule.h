#pragma once
#include "../Scripting/ScriptingManager.h"

class EntityContext;

class EntityModule {
public:
    virtual ~EntityModule() = default;

    EntityModule();

    virtual void registerComponents(EntityContext &) {};

    virtual void registerSystems(EntityContext &) {};

    virtual void registerScriptableTypes(ScriptingManager &) {};

    virtual void postRegister(EntityContext &) {};
};
