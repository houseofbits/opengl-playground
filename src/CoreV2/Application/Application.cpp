#include "Application.h"
#include "../../Helper/Log.h"
#include "../../ModulesV2/Common/CommonModule.h"
#include "../../ModulesV2/Renderer/RendererModule.h"
#include <fstream>

Application::Application() : m_EntityContext(), m_EntitySourceFileName() {
}

void Application::initialize(const std::string &entityDefinitionFileName) {
    std::ifstream file(entityDefinitionFileName);
    if (file.fail()) {
        Log::error("Application::Application: Failed to read " + entityDefinitionFileName);
        return;
    }
    auto json = nlohmann::json::parse(file);
    m_EntityContext.deserializeEntityMap(json);

    m_EntityContext.registerModule<CommonModule>();
    m_EntityContext.registerModule<RendererModule>();
}

void Application::loadEntitiesFromFile(const std::string& fileName) {
    m_EntitySourceFileName = fileName;
    std::ifstream file(fileName);
    if (file.fail()) {
        Log::error("Application::loadEntitiesFromFile: Failed to read " + fileName);
        return;
    }
    auto json = nlohmann::json::parse(file);
    m_EntityContext.deserializeEntities(json);
    m_EntityContext.registerEntitiesWithSystems();
    m_EntityContext.initializeSystems();
}

void Application::run() {

}
