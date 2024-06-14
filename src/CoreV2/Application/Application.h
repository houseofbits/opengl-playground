#pragma once

#include "../Entity/EntityContext.h"

class Application {
public:
    Application();

    virtual void run();

    void initialize(const std::string& entityDefinitionFileName);
    void loadEntitiesFromFile(const std::string& fileName);

protected:
    EntityContext m_EntityContext;
    std::string m_EntitySourceFileName;
};
