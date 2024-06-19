#include "MainApplication.h"
#include "../../ModulesV2/Common/CommonModule.h"
#include "../../ModulesV2/Renderer/RendererModule.h"
#include <fstream>

MainApplication::MainApplication() : Application(), m_Window(&m_EventManager) {
    m_EventManager.registerEventReceiver(this, &MainApplication::handleInputEvent);
}

void MainApplication::initialize(const std::string &entityDefinitionFileName) {
    std::ifstream file(entityDefinitionFileName);
    if (file.fail()) {
        Log::error("Application::Application: Failed to read " + entityDefinitionFileName);
        return;
    }
    auto json = nlohmann::json::parse(file);
    m_EntityContext.deserializeEntityMap(json);

    m_EntityContext.registerModule<CommonModule>();
    m_EntityContext.registerModule<RendererModule>();

    m_Window.create();

    m_EntityContext.initializeSystems(&m_EventManager);
}

void MainApplication::run() {


        while (true) {
              m_ResourceManager.buildFetchedResources();
            if (!m_Window.pollEvents() || !m_EventManager.processEvents()) {
                break;
            }
            m_EntityContext.processSystems();

            m_Window.doubleBuffer();
        }

        m_Window.destroy();
}

bool MainApplication::handleInputEvent(InputEvent *const event) {
    if (event->type == InputEvent::KEYDOWN) {
        std::cout << event->keyCode << std::endl;
    }

    return true;
}