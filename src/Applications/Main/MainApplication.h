#pragma once

#include "../../Core/API.h"
#include "../../Window.h"

class MainApplication : public Application, public EventHandler {
public:
    MainApplication();

    void run() override;

    void initialize(const std::string &entityDefinitionFileName);

    void handleInputEvent(const InputEvent&);

    void handleEntityCreationEvent(const EntityCreationEvent&);

    //Used for event manager to identify target handlers
    Identity::Type getId() override { return 0; }

    Window m_Window;
};
