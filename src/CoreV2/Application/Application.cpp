#include "Application.h"
#include <fstream>

Application::Application() : m_EventManager(), m_ResourceManager(), m_EntityContext(), m_EntitySourceFileName() {
}

void Application::loadEntitiesFromFile(const std::string &fileName) {
    m_EntitySourceFileName = fileName;
    std::ifstream file(fileName);
    if (file.fail()) {
        Log::error("Application::loadEntitiesFromFile: Failed to read " + fileName);
        return;
    }
    auto json = nlohmann::json::parse(file);
    m_EntityContext.deserializeEntities(json, m_ResourceManager);
}

void Application::run() {
}
