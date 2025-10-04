#pragma once

class EntityContext;
class ScriptingManager;
class ResourceFactory;

class EntityModule {
public:
    virtual ~EntityModule() = default;

    EntityModule();

    virtual void registerComponents(EntityContext &) {
    };

    virtual void registerSystems(EntityContext &) {
    };

    virtual void registerResources(ResourceFactory &) {
    };

    virtual void registerScriptableTypes(ScriptingManager &) {
    };

    virtual void postRegister(EntityContext &) {
    };
};
