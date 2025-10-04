#pragma once

#include "../Entity/EntityContext.h"
// #include "../Events/EventManager.h"
#include "../Events/EventManager_V2.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/ResourceProcess.h"
#include <string>

class Application {
public:
    virtual ~Application() = default;

    Application();

    virtual void run();

    void loadEntitiesFromFile(const std::string &fileName);

    void saveEntitiesToFile() const;

    template<class T, typename = std::enable_if_t<std::is_base_of_v<EntityModule, T> > >
    T *registerModule() {
        auto *p = new T();
        m_Modules.push_back(p);
        m_Modules.back()->registerComponents(m_EntityContext);
        m_Modules.back()->registerSystems(m_EntityContext);
        m_Modules.back()->registerResources(m_ResourceManager.resourceFactory);
        m_Modules.back()->registerScriptableTypes(m_EntityContext.scriptingManager);

        return p;
    }

    void postRegisterModules();

protected:
    std::list<EntityModule *> m_Modules;
    EventManager m_EventManager;
    ResourceManager m_ResourceManager;
    EntityContext m_EntityContext;
    std::string m_EntitySourceFileName;
    ResourceProcess m_resourceProcess;
};
