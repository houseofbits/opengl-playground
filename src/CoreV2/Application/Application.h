#pragma once

#include "../Entity/EntityContext.h"
#include "../Events/EventManager.h"

class Application {
public:
    Application();

    virtual void run();

    void loadEntitiesFromFile(const std::string& fileName);

protected:
    EventManager m_EventManager;
    ResourceManager m_ResourceManager;
    EntityContext m_EntityContext;
    std::string m_EntitySourceFileName;
};
