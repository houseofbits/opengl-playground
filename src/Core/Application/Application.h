#pragma once

#include "../Entity/EntityContext.h"
#include "../Events/EventManager.h"
#include "../Resources/ResourceManager.h"
#include <string>

class Application {
public:
    Application();

    virtual void run();

    void loadEntitiesFromFile(const std::string &fileName);
    void saveEntitiesToFile();

protected:
    EventManager m_EventManager;
    ResourceManager m_ResourceManager;
    EntityContext m_EntityContext;
    std::string m_EntitySourceFileName;
};
